#include "Geodude.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "p2Defs.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Item.h"
#include "j1Collision.h"
#include "j1EntityElementsScene.h"
#include "j1Audio.h"
#include "j1Weapon.h"

Geodude::Geodude()
{
	type = CREATURE;
}

Geodude::~Geodude()
{
	collision_feet = nullptr;
}

bool Geodude::Awake(pugi::xml_node &conf, uint id, iPoint pos)
{
	name = conf.attribute("name").as_string("");
	hp = conf.attribute("hp").as_int(0);
	attack = conf.attribute("attack").as_int(0);
	speed = conf.attribute("speed").as_int(0);
	std::string temp = conf.attribute("dir").as_string("");
	if (temp == "up")
		direction = UP;
	else if (temp == "down")
		direction = DOWN;
	else if (temp == "left")
		direction = LEFT;
	else
		direction = RIGHT;

	mode_stone = conf.attribute("mode_stone").as_bool(false);
	if(mode_stone)
		position = iPoint(pos.x, pos.y);
	else
		position = iPoint(conf.attribute("pos_x").as_int(0), conf.attribute("pos_y").as_int(0));

	return true;
}

bool Geodude::Start()
{
	state = P_IDLE;
	anim_state = P_IDLE;
	scale = App->win->GetScale();
	offset_x = 8;
	offset_y = 17;
	movable = true;
	collision_feet = App->collision->AddCollider({ position.x + offset_x, position.y + offset_y, 15, 15 }, COLLIDER_POKEMON, this);
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	reset_run = true;

	//Get the animations
	animation = *App->anim_manager->GetAnimStruct(GEODUDE);

	return true;
}

bool Geodude::Update(float dt)
{
	BROFILER_CATEGORY("Update_Collision", Profiler::Color::DimGray);

	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME)
	{
		switch (state)
		{
		case P_IDLE:
		{
			Idle();
			break;
		}
		case P_WALKING:
		{
			Walking(dt);
			break;
		}
		case P_ATTACKING:
		{
			Attack();
			break;
		}
		case P_HIT:
		{
			Movebyhit(dt);
			break;
		}
		case P_DYING:
		{
			Death();
			break;
		}
		default:
		{
			break;
		}
		}
	}

	else if (App->scene->gamestate == INMENU)
	{

	}
	/*else if (App->scene->gamestate == TIMETOPLAY)
	{
		if (SDL_GetTicks() - timetoplay > 1000)
		{
			App->scene->gamestate = INGAME;
		}
	}*/

	//Collision follow the player
	if (collision_feet != nullptr)
	{
		collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
	}
	return true;
}

void Geodude::Draw()
{
	BROFILER_CATEGORY("Draw_Geodude", Profiler::Color::Yellow);

	if (direction == UP)
	{
		anim_rect = animation.anim[anim_state].North_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].North_action.GetCurrentOffset();
	}
	else if (direction == DOWN)
	{
		anim_rect = animation.anim[anim_state].South_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].South_action.GetCurrentOffset();
	}
	else if (direction == LEFT)
	{
		anim_rect = animation.anim[anim_state].West_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].West_action.GetCurrentOffset();
	}
	else if (direction == RIGHT)
	{
		anim_rect = animation.anim[anim_state].East_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].East_action.GetCurrentOffset();
	}

	//DRAW
	App->render->Blit(animation.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);
}

bool Geodude::CleanUp()
{
	return false;
}

bool Geodude::Idle()
{
	if (movable)
	{
		if (reset_run)
		{
			timetorun = SDL_GetTicks();
			reset_run = false;
		}
		else
		{
			if (SDL_GetTicks() - timetorun > 2000)
			{
				int direc_select = rand() % 4 + 1;
				if (direc_select == 1)
				{
					direction = UP;
				}
				else if (direc_select == 2)
				{
					direction = DOWN;
				}
				else if (direc_select == 3)
				{
					direction = LEFT;
				}
				else if (direc_select == 4)
				{
					direction = RIGHT;
				}
				state = P_WALKING;
				anim_state = P_WALKING;
				reset_distance = true;
			}
		}
	}
	return true;
}

bool Geodude::Walking(float dt)
{
	walking = false;
	if (reset_distance)
	{
		distance = rand() % 60 + 20;
		dis_moved = 0;
		reset_distance = false;
	}
	Move(dt);

	if (dis_moved >= distance)
	{
		walking = false;
		reset_run = true;
	}


	if (walking == false)
	{
		state = P_IDLE;
		anim_state = P_IDLE;
	}

	else
	{
		state = P_WALKING;
		anim_state = P_WALKING;
	}
	return true;
}

bool Geodude::Move(float dt)
{
	if (direction == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - ceil(speed*dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= ceil(speed*dt);
			dis_moved++;
		}
		else
		{
			dis_moved++;
		}
		walking = true;
	}

	if (direction == RIGHT)
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + ceil(speed*dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += ceil(speed*dt);
			dis_moved++;
		}
		else
		{
			dis_moved++;
		}
		walking = true;
	}
	if (direction == UP)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - ceil(speed*dt), collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			position.y -= ceil(speed*dt);
			dis_moved++;
		}
		else
		{
			dis_moved++;
		}
		walking = true;
	}
	if (direction == DOWN)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + ceil(speed*dt), collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += ceil(speed*dt);
			dis_moved++;
		}
		else
		{
			dis_moved++;
		}
		walking = true;
	}

	return true;
}


bool Geodude::Attack()
{
	//CHECK IF ATTACK ANIMATION IS FINISHED
	if (animation.anim[P_ATTACKING].East_action.Finished() || 
		animation.anim[P_ATTACKING].West_action.Finished() ||
		animation.anim[P_ATTACKING].North_action.Finished() ||
		animation.anim[P_ATTACKING].South_action.Finished())
	{
		state = P_IDLE;
		anim_state = P_IDLE;
	}

	return true;
}

bool Geodude::Death()
{
	if (App->scene->player->bombmanager != nullptr)
	{
		iPoint pos;
		pos.create(position.x - offset_x, position.y - offset_y);
		App->entity_elements->CreateItem(2, pos);
	}

	to_delete = true;
	return true;
}

bool Geodude::Movebyhit(float dt)
{
	if (hp <= 0) 
	{
		state = P_DYING;
		anim_state = P_DYING;
		return true;
	}

	if (knockback_time.ReadSec() >= 0.2)
	{
		state = P_IDLE;
		anim_state = P_IDLE;
		return true;
	}

	if (hurt_timer.ReadSec() >= 0.2)
	{
		state = P_IDLE;
		anim_state = P_IDLE;
		return true;
	}

	if (dir_hit == UP)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - ceil(90 * dt), collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			position.y -= ceil(90 * dt);
		}
	}
	else if (dir_hit == DOWN)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + ceil(90 * dt), collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += ceil(90 * dt);
		}
	}
	else if (dir_hit == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - ceil(90 * dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= ceil(90 * dt);
		}
	}
	else if (dir_hit == RIGHT)
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + ceil(90 * dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += ceil(90 * dt);
		}
	}

	return true;
}

void Geodude::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr && state != P_DYING)
	{
		//SWORD COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_SWORD && state != P_HIT)
		{
			if (c2->callback != nullptr)
			{
				knockback_time.Start();
				animation.anim[P_DYING].ResetAnimations();
				hurt_timer.Start();
				state = P_HIT;
				anim_state = P_DYING;
				dir_hit = c2->callback->direction;
				prev_position = position;
				hp--;
			}
		}

		//PLAYER COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_PLAYER && c2->callback != nullptr)
		{
			if (((Player*)c2->callback)->GetState() != L_HIT && ((Player*)c2->callback)->GetState() != L_HOOKTHROWN)
			{
				if (state != P_ATTACKING && state != L_HIT)
				{
					App->audio->PlayFx(13);
					Player* player = (Player*)c2->callback;
					state = P_ATTACKING;
					anim_state = P_ATTACKING;
					animation.anim[anim_state].ResetAnimations();
					Orientate();
					player->SetState(L_HIT);
					player->SetAnimState(L_IDLE);
					player->invincible_timer.Start();
					player->hurt_timer.Start();
					player->GetDamage();
					player->dir_hit = direction;
					player->prev_position = player->position;
				}
			}
		}

		//ARROW COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_ARROW && c2->arrow_callback != nullptr)
		{
			if (c2->arrow_callback->step == AIR && state != P_HIT)
			{
				knockback_time.Start();
				animation.anim[P_DYING].ResetAnimations();
				hurt_timer.Start();
				state = P_HIT;
				anim_state = P_DYING;
				dir_hit = c2->arrow_callback->direction;
				hp--;
				c2->arrow_callback->step = ENEMY_IMPACT; // TODO MED -> set step to impact: this will reproduce the impact animation and, when finished, set step to DIE.
				prev_position = position;
			}
		}
		// HIT BY BOMB -------------------
		if (c1 == collision_feet && c2->type == COLLIDER_BOMB)
		{
			if (state != P_HIT)
			{
				knockback_time.Start();
				animation.anim[P_DYING].ResetAnimations();
				hurt_timer.Start();
				state = P_HIT;
				anim_state = P_DYING;
				SetKnockbackDir();
				prev_position = position;
				hp--;
			}
		}
		// --------------------------------------
	}
}

void Geodude::SetKnockbackDir()
{
	if (direction == UP)
	{
		dir_hit = DOWN;
	}
	else if (direction == DOWN)
	{
		dir_hit = UP;
	}
	else if (direction == LEFT)
	{
		dir_hit = RIGHT;
	}
	else
	{
		dir_hit = LEFT;
	}
}
