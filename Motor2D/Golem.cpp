#include "Golem.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "p2Defs.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Item.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1EntityElementsScene.h"
#include "j1Audio.h"
#include "j1Weapon.h"

Golem::Golem()
{
	type = CREATURE;
}

Golem::~Golem()
{
}

bool Golem::Awake(pugi::xml_node &conf, uint id)
{
	name = conf.attribute("name").as_string("");
	hp = conf.attribute("hp").as_int(0);
	attack = conf.attribute("name").as_int(0);
	speed = conf.attribute("name").as_int(0);
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
	position = iPoint(conf.attribute("pos_x").as_int(0), conf.attribute("pos_y").as_int(0));

	return true;
}

bool Golem::Start()
{
	state = P_STATIC;
	anim_state = P_STATIC;
	scale = App->win->GetScale();
	offset_x = 7;
	offset_y = 14;
	movable = true;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 14, 14 }, COLLIDER_POKEMON, this);
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	reset_run = true;

	//Modify Meta
	iPoint temp = App->map->WorldToMap(position.x, position.y);
	App->map->EditCost(temp.x, temp.y, App->map->data.tilesets[0]->firstgid + 1);
	App->map->EditCost(temp.x - 1, temp.y, App->map->data.tilesets[0]->firstgid + 1);
	App->map->EditCost(temp.x, temp.y - 1, App->map->data.tilesets[0]->firstgid + 1);
	App->map->EditCost(temp.x - 1, temp.y - 1, App->map->data.tilesets[0]->firstgid + 1);

	//Get the animations
	animation = *App->anim_manager->GetAnimStruct(GOLEM); //id 5 = Golem

	// Test for Vertical Slice /// TODO MED-> read stats from XML
	radar = 40;
	chase_speed = 60;

	return true;
}

bool Golem::Update(float dt)
{
	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME)
	{
		switch (state)
		{
		case P_IDLE:
		{
			CheckPlayerPos();
			Idle();
			break;
		}
		/*case WALKING:
		{
			CheckPlayerPos();
			Walking();
			break;
		}*/
		case P_CHASING:
		{
			CheckPlayerPos();
			Chase(dt);
			break;
		}
		case P_ATTACKING:
		{
			Attack();
			break;
		}
		case P_HIT:
		{
			Hit(dt);
			break;
		}
		case P_STATIC:
		{
			break;
		}

		/* This case should be added, it reffers when abandoning rock stage */
		case P_SPECIAL:
		{
			if (mode_stone)
			{
				mode_stone = false;
				timetoplay = SDL_GetTicks();
			}
			if (mode_stone == false && SDL_GetTicks() - timetoplay > 500)
			{
				state = P_IDLE;
				anim_state = P_IDLE;
			}
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
	collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
	return true;
}

void Golem::Draw()
{
	BROFILER_CATEGORY("Draw_SOLDIER", Profiler::Color::Yellow);
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

bool Golem::CleanUp()
{
	return true;
}

bool Golem::CheckPlayerPos()
{
	int distance_player = App->scene->player->position.DistanceTo(position);

	if (distance_player <= radar && App->scene->player->invincible_timer.ReadSec() >= 1)
	{
		state = P_CHASING;
		anim_state = P_WALKING;
	}
	else
	{
		state = P_IDLE;
		anim_state = P_IDLE;
	}
	return true;
}

bool Golem::Idle()
{
	if (movable)
	{
		if (change_dir.ReadSec() > 2)
		{
			change_dir.Start();
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
		}
	}

	return true;
}

/*bool Golem::Walking()
{
	walking = false;
	if (reset_distance)
	{
		distance = rand() % 60 + 20;
		dis_moved = 0;
		reset_distance = false;
	}

	Move();

	if (dis_moved >= distance)
	{
		walking = false;
		reset_run = true;
	}


	if (walking == false)
	{
		state = IDLE;
	}
	else
	{
		state = WALKING;
	}
	return true;
}*/

bool Golem::Move(float dt)
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

bool Golem::Chase(float dt)
{
	if(App->scene->player->invincible_timer.ReadSec() > 1)
	{
		int distance_player = App->scene->player->position.DistanceTo(position);

		if (distance_player <= radar)
		{
			iPoint player_pos = App->map->WorldToMap(App->scene->player->position.x, App->scene->player->position.y);
			GoTo(player_pos, ceil(dt*chase_speed));
			Orientate();
			state = P_CHASING;
			anim_state = P_WALKING;
		}
		else
		{
			state = P_IDLE;
			anim_state = P_IDLE;
		}
	}
	return true;
}

bool Golem::Hit(float dt)
{
	if(hp <= 0)
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
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - ceil(40 * dt), collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			position.y -= ceil(40 * dt);
		}
	}
	else if (dir_hit == DOWN)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + ceil(40 * dt), collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += ceil(40 * dt);
		}
	}
	else if (dir_hit == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - ceil(40 * dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= ceil(40 * dt);
		}
	}
	else if (dir_hit == RIGHT)
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + ceil(40 * dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += ceil(40 * dt);
		}
	}

	return true;
}

bool Golem::Attack()
{
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

bool Golem::Death()
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

void Golem::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		//BOMB COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_BOMB)
		{
			if (state == P_STATIC)
			{
				state = P_SPECIAL;
				anim_state = P_SPECIAL;
				iPoint temp = App->map->WorldToMap(position.x, position.y);
				App->map->EditCost(temp.x, temp.y, 0);
				App->map->EditCost(temp.x - 1, temp.y, 0);
				App->map->EditCost(temp.x, temp.y - 1, 0);
				App->map->EditCost(temp.x - 1, temp.y - 1, 0);

			}
		}

		//SWORD COLLISION
		if(c1 == collision_feet && c2->type == COLLIDER_SWORD && state != P_HIT && state != P_STATIC && state != P_SPECIAL)
		{
			if (c2->callback != nullptr)
			{
				knockback_time.Start();
				animation.anim[P_DYING].ResetAnimations();
				hurt_timer.Start();
				dir_hit = c2->callback->direction;
				state = P_HIT;
				anim_state = P_DYING;
				hp--;
			}
		}

		//ARROW COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_ARROW && c2->arrow_callback != nullptr)
		{
			if (c2->arrow_callback->step == AIR && state != P_HIT)
			{
				App->audio->PlayFx(12);
				knockback_time.Start();
				hp--;
				state = P_HIT;
				anim_state = P_IDLE;
				dir_hit = c2->arrow_callback->direction;
				prev_position = position;
				c2->arrow_callback->step = IMPACT; 
			}
		}

		//PLAYER COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_PLAYER && state != P_STATIC)
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
	}
}