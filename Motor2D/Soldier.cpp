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

Soldier::Soldier():NPC()
{
	name = "enemies"; //TODO need change name to "Soldier".
	type = CREATURE;
	srand(time(NULL));
}

Soldier::~Soldier()
{}

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

bool Soldier::Start()
{
	if (soldier_type == AGGRESSIVE)
	{
		offset_x = 8;
		offset_y = 15;
		//marge = 12;
		gamestate = TIMETOPLAY;
		state = IDLE;
		speed = 1;
		//offset_x = 15;
		//offset_y = 15;
		timetoplay = SDL_GetTicks();
		reset_distance = false;
		reset_run = true;

		// Test for Vertical Slice /// TODO MED-> read stats from XML
		radar = 75;
		attack_speed = 1;
		chase_speed = 1;
	}

	else if (soldier_type == PASSIVE)
	{
		offset_x = 8;
		offset_y = 15;
	}
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 16, 15 }, COLLIDER_ENEMY, this);

	//Get the animations
	animation = *App->anim_manager->GetAnimStruct(6); //id 6 = soldier
	return true;
}

bool Soldier::Update()
{
	BROFILER_CATEGORY("DoUpdate_Soldier", Profiler::Color::Pink)
	// STATE MACHINE ------------------
		if (gamestate == INGAME)
		{
			if (soldier_type == AGGRESSIVE)
			{
				switch (state)
				{
				case IDLE:
				{
					CheckPlayerPos();
					Idle();
					break;
				}
				case WALKING:
				{
					CheckPlayerPos();
					Walking();
					break;
				}
				case DYING:
				{
					Die();
					break;
				}
				case HIT:
				{
					Movebyhit();
					break;
				}
				case CHASING:
				{
					CheckPlayerPos();
					Chase();
					break;
				}
				case ATTACKING:
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

		collision_feet->SetPos(position.x - offset_x, position.y - offset_y);

	}
	else if (gamestate == INMENU)
	{

	}
	else if (gamestate == TIMETOPLAY)
	{
		if (SDL_GetTicks() - timetoplay > 1000)
		{
			gamestate = INGAME;
		}
	}
	return true;
}

void Soldier::Draw()
{
	BROFILER_CATEGORY("Draw_SOLDIER", Profiler::Color::Yellow)
		//App->anim_manager->Drawing_Manager(state, direction, position, 6);
		if (soldier_type == PASSIVE)
		{
			id = 2;
		}
		else
		{
			switch (state)
			{
			case IDLE:
				id = 0;
				break;
			case WALKING:
				id = 1;
				break;
			case HIT:
				id = 1;
				break;
			case CHASING:
			{
				id = 1;
				break;
			}
			default:
				break;
			}
		}

	if (direction == UP)
	{
		anim_rect = animation.anim[id].North_action.GetCurrentFrame();
		pivot = animation.anim[id].North_action.GetCurrentOffset();
	}
	else if (direction == DOWN)
	{
		anim_rect = animation.anim[id].South_action.GetCurrentFrame();
		pivot = animation.anim[id].South_action.GetCurrentOffset();
	}
	else if (direction == LEFT)
	{
		anim_rect = animation.anim[id].West_action.GetCurrentFrame();
		pivot = animation.anim[id].West_action.GetCurrentOffset();
	}
	else if (direction == RIGHT)
	{
		anim_rect = animation.anim[id].East_action.GetCurrentFrame();
		pivot = animation.anim[id].East_action.GetCurrentOffset();
	}

	App->render->Blit(animation.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);
	
}

void Soldier::AddItem(Item* item)
{
	item_inside = item;
	item->canBlit = false;
}

void Soldier::Drop_item()
{
	item_inside->canBlit = true;
	item_inside->position.x = position.x - item_inside->collision->rect.w;
	item_inside->position.y = position.y - item_inside->collision->rect.h;
	item_inside = NULL;
}

bool Soldier::CheckPlayerPos()
{
	int distance_player = App->scene->player->position.DistanceTo(position);

	if (distance_player <= radar && App->scene->player->invincible_timer.ReadSec() >= 1)
	{
		state = CHASING;
	}
	else
	{
		state = IDLE;
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
				state = WALKING;
				reset_distance = true;
			}
		}
	}
	return true;
}

bool Soldier::Walking()
{
	walking = false;

	if (reset_distance)
	{
		distance = rand() % 100 + 20;
		dis_moved = 0;
		reset_distance = false;
	}

	Move();

	if(dis_moved >= distance)
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
}

bool Soldier::Move()
{
	if (direction == LEFT)
	{
		//App->map->MovementCost(position.x - speed, position.y, LEFT)
		if (App->map->MovementCost(collision_feet->rect.x - speed, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= speed;
			dis_moved++;
		}
		else
		{
			//Function to change direction
			dis_moved++;
		}
		walking = true; 
	}

	if (direction == RIGHT)
	{
		//App->map->MovementCost(position.x + (speed + width), position.y, RIGHT)
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + speed, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += speed;
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
		//App->map->MovementCost(position.x, position.y - speed, UP)
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - speed, collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			position.y -= speed;
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
		//App->map->MovementCost(position.x, position.y + (speed + height), DOWN)
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + speed, collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += speed;
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

bool Soldier::Chase()
{
	//path.clear();
	//attack_time.Start();

	if (App->scene->player->state != HIT)
	{
		iPoint player_pos = App->map->WorldToMap(App->scene->player->position.x, App->scene->player->position.y);
		GoTo(player_pos, chase_speed);
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
	App->audio->PlayFx(11);
	App->scene->items.push_front(App->entity_elements->CreateItem(item_id, position));
	App->entity_elements->DeleteEnemy(this);
	return true;
}

bool Soldier::Movebyhit()
{
	if (hp <= 0)
	{
		state = DYING;
		return true;
	}

	if (knockback_time.ReadSec() >= 0.2)
	{
		state = CHASING;
		return true;
	}

	if (dir_hit == UP)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - 4, collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			position.y -= 4;
		}
	}
	else if (dir_hit == DOWN)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + 4, collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += 4;
		}
	}
	else if (dir_hit == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - 4, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= 4;
		}
	}
	else if (dir_hit == RIGHT)
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + 4, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += 4;
		}
	}
	/*if (position.x > (previus_position.x + 65) ||
	position.x < (previus_position.x + 65) ||
	position.y >(previus_position.y + 65) ||
	position.y < (previus_position.y + 65))
	{
	state = IDLE;
	}*/
	return true;
}

