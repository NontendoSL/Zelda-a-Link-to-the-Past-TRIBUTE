#include "Soldier.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "p2Defs.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Item.h"
#include "j1Collision.h"
#include "j1EntityElementsScene.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "j1Weapon.h"
#include "j1DynamicObjects.h"
#include "Animation.h"

Soldier::Soldier():NPC()
{
	name = "Soldier";
	type = CREATURE;
	srand(time(NULL));
}

Soldier::~Soldier()
{
	collision_feet = nullptr;
}

bool Soldier::Awake(pugi::xml_node &conf, uint id)
{
	bool stop_search = false;
	for (int s_id = conf.attribute("id").as_int(0); stop_search == false; s_id = conf.next_sibling().attribute("id").as_int(0))
	{
		if (id == s_id)
		{
			hp = conf.attribute("hp").as_int(0);
			position.x = conf.attribute("pos_x").as_int(0);
			position.y = conf.attribute("pos_y").as_int(0);
			std::string temp = conf.attribute("file").as_string("");

			item_id = conf.attribute("item_id").as_int(0);

			temp = conf.attribute("dir").as_string("");
			if (temp == "up")
				direction = UP;
			else if (temp == "down")
				direction = DOWN;
			else if (temp == "left")
				direction = LEFT;
			else
				direction = RIGHT;

			movable = conf.attribute("canMove").as_bool(false);
			destructible = conf.attribute("destructible").as_bool(false);

			if (destructible == false)
			{
				soldier_type = PASSIVE;
			}
			else
			{
				soldier_type = AGGRESSIVE;
			}

			npc_id = id;
			stop_search = true;
		}
	}

	return true;
}

bool Soldier::CleanUp()
{
	return true;
}

void Soldier::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		//SWORD COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_SWORD && c1->callback != nullptr)
		{
			if (destructible == true && state != S_HIT && state != S_DYING)
			{
				App->audio->PlayFx(12);
				knockback_time.Start();
				hp--;
				state = S_HIT;
				anim_state = S_IDLE;
				dir_hit = c2->callback->direction;
				prev_position = position;
			}
		}

		//ARROW COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_ARROW && c2->arrow_callback != nullptr)
		{
			if (c2->arrow_callback->step == AIR && destructible == true && state != S_HIT)
			{
				App->audio->PlayFx(12);
				knockback_time.Start();
				hp--;
				state = S_HIT;
				anim_state = S_IDLE;
				dir_hit = c2->arrow_callback->direction;
				prev_position = position;
				c2->arrow_callback->step = IMPACT; // TODO MED -> set step to impact: this will reproduce the impact animation and, when finished, set step to DIE.
			}
		}

		//DYNOBJECT COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_DYNOBJECT && c2->callback != nullptr)
		{
			if (((DynamicObjects*)c2->callback)->GetState() == D_AIR && soldier_type != PASSIVE)
			{
				App->audio->PlayFx(12);
				knockback_time.Start();
				hp -= 2; // TODO LOW -> set attack dmg to each type of dynobject.
				state = S_HIT;
				anim_state = S_IDLE;
				dir_hit = c2->callback->direction;
				prev_position = position;
				((DynamicObjects*)c2->callback)->SetState(D_DYING);
			}
		}
	}
}

bool Soldier::Start()
{
	if (soldier_type == AGGRESSIVE)
	{
		offset_x = 8;
		offset_y = 15;
		state = S_IDLE;
		anim_state = S_IDLE;
		speed = 40;
		timetoplay = SDL_GetTicks();
		reset_distance = false;
		reset_run = true;
		radar = 75;
		chase_speed = 50;
		death_graphics = App->tex->Load("textures/AnimationsAndEffects.png");

	}

	else if (soldier_type == PASSIVE)
	{
		offset_x = 8;
		offset_y = 15;
		anim_state = S_GUARD;
	}

	//Set collider
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 16, 15 }, COLLIDER_ENEMY, this);

	//Get the animations
	animation = *App->anim_manager->GetAnimStruct(SOLDIER);

	return true;
}

bool Soldier::Update(float dt)
{
	BROFILER_CATEGORY("DoUpdate_Soldier", Profiler::Color::Pink);
	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME)
	{
		if (soldier_type == AGGRESSIVE)
		{
			switch (state)
			{
			case S_IDLE:
			{
				CheckPlayerPos();
				Idle();
				break;
			}
			case S_WALKING:
			{
				CheckPlayerPos();
				Walking(dt);
				break;
			}
			case S_DYING:
			{
				Die();
				break;
			}
			case S_HIT:
			{
				Movebyhit(dt);
				break;
			}
			case S_CHASING:
			{
				CheckPlayerPos();
				Chase(dt);
				break;
			}
			case S_ATTACKING:
			{
				Attack();
				break;
			}
			default:
			{
				break;
			}
			}
		}
		if (collision_feet != nullptr)
		{
			collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
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
	return true;
}

void Soldier::Draw()
{
	BROFILER_CATEGORY("Draw_SOLDIER", Profiler::Color::Yellow);
	//App->anim_manager->Drawing_Manager(state, direction, position, 6);

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

	App->render->Blit(animation.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);

}

bool Soldier::CheckPlayerPos()
{
	int distance_player = App->scene->player->position.DistanceTo(position);

	if (distance_player <= radar && App->scene->player->invincible_timer.ReadSec() >= 1)
	{
		state = S_CHASING;
		anim_state = S_WALKING;
	}
	else
	{
		state = S_IDLE;
		anim_state = S_IDLE;
	}

	return true;
}

bool Soldier::Idle()
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
				state = S_WALKING;
				anim_state = S_WALKING;
				reset_distance = true;
			}
		}
	}
	return true;
}

bool Soldier::Walking(float dt)
{
	walking = false;

	if (reset_distance)
	{
		distance = rand() % 100 + 20;
		dis_moved = 0;
		reset_distance = false;
	}

	Move(dt);

	if(dis_moved >= distance)
	{
		walking = false;
		reset_run = true;
	}


	if (walking == false)
	{
		state = S_IDLE;
		anim_state = S_IDLE;
	}

	else
	{
		state = S_WALKING;
		anim_state = S_WALKING;
	}

	return true;
}

bool Soldier::Move(float dt)
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

bool Soldier::Chase(float dt)
{
	//path.clear();
	//attack_time.Start();

	if (App->scene->player->GetState() != L_HIT)
	{
		iPoint player_pos = App->map->WorldToMap(App->scene->player->position.x, App->scene->player->position.y);
		GoTo(player_pos, ceil(dt*chase_speed));
		Orientate();
	}
	return true;
}

bool Soldier::Attack()
{


	return true;
}

bool Soldier::Die()
{
	if (animation.anim[anim_state].South_action.Finished() == true)
	{
		App->entity_elements->CreateItem(DropItem(), position);
		to_delete = true;
	}
	return true;
}

bool Soldier::Movebyhit(float dt)
{
	if (hp <= 0)
	{
		App->audio->PlayFx(11);
		state = S_DYING;
		anim_state = S_DYING;
		animation.anim[S_DYING].ResetAnimations();
		direction = DOWN;

		//Erase Collider
		collision_feet->to_delete = true;
		collision_feet = nullptr;

		if (death_graphics != nullptr)
		{
			animation.graphics = death_graphics;
		}
	}

	else
	{
		if (knockback_time.ReadSec() >= 0.2)
		{
			state = S_CHASING;
			anim_state = S_WALKING;
			return true;
		}

		if (dir_hit == UP)
		{
			if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - ceil(240 * dt), collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
			{
				position.y -= ceil(240 * dt);
			}
		}
		else if (dir_hit == DOWN)
		{
			if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + ceil(240 * dt), collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
			{
				position.y += ceil(240 * dt);
			}
		}
		else if (dir_hit == LEFT)
		{
			if (App->map->MovementCost(collision_feet->rect.x - ceil(240 * dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
			{
				position.x -= ceil(240 * dt);
			}
		}
		else if (dir_hit == RIGHT)
		{
			if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + ceil(240 * dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
			{
				position.x += ceil(240 * dt);
			}
		}
	}

	return true;
}

SoldierState Soldier::GetState() const
{
	return state;
}

void Soldier::SetState(SoldierState s_state)
{
	state = s_state;
}

void Soldier::SetAnimState(SoldierState a_state)
{
	anim_state = a_state;
}

SoldierType Soldier::GetType() const
{
	return soldier_type;
}

int Soldier::DropItem()
{
	if (soldier_type == AGGRESSIVE)
	{
		int id = rand() % 100 + 1;

		if (id > 0 && id <= 10)
		{
			item_id = 1;
		}
		else if (id > 10 && id <= 30)
		{
			item_id = 7;
		}
		else
		{
			item_id = 0;
		}
	}
	return item_id;
}

