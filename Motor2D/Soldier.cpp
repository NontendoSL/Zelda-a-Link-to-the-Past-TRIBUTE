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
			std::string temp = conf.attribute("file").as_string("");
			texture = App->tex->Load(temp.c_str());
			hp = conf.attribute("hp").as_int(0);
			position.x = conf.attribute("pos_x").as_int(0);
			position.y = conf.attribute("pos_y").as_int(0);
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
	offset_x = 8;
	offset_y = 15;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 15, 15 }, COLLIDER_ENEMY, this);

	//marge = 12;
	gamestate = TIMETOPLAY;
	status_action = GUARD;
	state = IDLE;
	speed = 1;
	//offset_x = 15;
	//offset_y = 15;
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	reset_run = true;

	//Get the animations of idle
	animation = *App->anim_manager->GetAnimStruct(6); //id 6 = soldier
	return true;
}

bool Soldier::Update()
{
	BROFILER_CATEGORY("DoUpdate_Soldier", Profiler::Color::Pink)
	// STATE MACHINE ------------------
	if (gamestate == INGAME)
	{
		if (status_action == GUARD)
		{
			switch (state)
			{
			case IDLE:
			{
				Idle();
				break;
			}
			case WALKING:
			{
				Walking();
				break;
			}
			case DYING:
			{
				Die();
			}
			case HIT:
			{
				Movebyhit();
			}
			default:
			{
				break;
			}
			}
		}
		else if (status_action == HUNT)
		{
			switch (state)
			{
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

	collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
	return true;
}

void Soldier::Draw()
{
	BROFILER_CATEGORY("Draw_SOLDIER", Profiler::Color::Yellow)
		//App->anim_manager->Drawing_Manager(state, direction, position, 6);
		test_state = state;
	if (test_state == HIT) // test state (we dont have hurt animation yet)
	{
		test_state = WALKING;
	}
	if (direction == UP)
	{
		anim_rect = animation.anim[test_state].North_action.GetCurrentFrame();
		pivot = animation.anim[test_state].North_action.GetCurrentOffset();
	}
	else if (direction == DOWN)
	{
		anim_rect = animation.anim[test_state].South_action.GetCurrentFrame();
		pivot = animation.anim[test_state].South_action.GetCurrentOffset();
	}
	else if (direction == LEFT)
	{
		anim_rect = animation.anim[test_state].West_action.GetCurrentFrame();
		pivot = animation.anim[test_state].West_action.GetCurrentOffset();
	}
	else if (direction == RIGHT)
	{
		anim_rect = animation.anim[test_state].East_action.GetCurrentFrame();
		pivot = animation.anim[test_state].East_action.GetCurrentOffset();
	}

	//DRAW
	if (texture == nullptr)
	{
		LOG("TEXTURE NULL");
	}
	else
	{
		App->render->Blit(animation.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);
	}
	/*if (npc_id == 1)
	{
	if (hp > 0)
	{
	if (direction == UP)
	{
	App->render->Blit(texture, position.x, position.y, &soldier_up);
	}
	if (direction == DOWN)
	{
	App->render->Blit(texture, position.x, position.y, &soldier_down);
	}
	if (direction == RIGHT)
	{
	App->render->Blit(texture, position.x, position.y, &soldier_right);
	}
	if (direction == LEFT)
	{
	App->render->Blit(texture, position.x, position.y, &soldier_left);
	}
	}
	}
	if (npc_id == 2)
	{
	if (hp > 0)
	{
	if (direction == UP)
	{
	App->render->Blit(texture, position.x, position.y, &soldier_up_2);
	}
	if (direction == DOWN)
	{
	App->render->Blit(texture, position.x, position.y, &soldier_down_2);
	}
	if (direction == RIGHT)
	{
	App->render->Blit(texture, position.x, position.y, &soldier_right_2);
	}
	if (direction == LEFT)
	{
	App->render->Blit(texture, position.x, position.y, &soldier_left_2);
	}
	}
	}*/

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
		if (App->map->MovementCost(position.x - offset_x, position.y, offset_x, offset_y, LEFT) == 0)
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
		if (App->map->MovementCost(position.x + offset_x, position.y, offset_x, offset_y, RIGHT) == 0)
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
		if (App->map->MovementCost(position.x, position.y - offset_y, offset_x, offset_y, UP) == 0)
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
		if (App->map->MovementCost(position.x, position.y + offset_y, offset_x, offset_y, DOWN) == 0)
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

bool Soldier::Attack()
{


	return true;
}

bool Soldier::Die()
{
	App->scene->items.push_back(App->entity_elements->CreateItem(1, position));
	App->entity_elements->DeleteEnemy(this);
	return true;
}

bool Soldier::Movebyhit()
{
	if (dir_hit == UP)
	{
		//App->map->MovementCost(position.x, position.y - speed, UP)
		if (App->map->MovementCost(position.x, position.y - offset_y, offset_x, offset_y, UP) == 0)
		{
			position.y -= 4;
		}
		if (position.y < (previus_position.y - 30))
		{
			state = IDLE;
		}
	}
	if (dir_hit == DOWN)
	{
		//App->map->MovementCost(position.x, position.y + (4 + height), DOWN)
		if (App->map->MovementCost(position.x, position.y + offset_y, offset_x, offset_y, DOWN) == 0)
		{
			position.y += 4;
		}

		if (position.y > (previus_position.y + 30))
		{
			state = IDLE;
		}
	}
	if (dir_hit == LEFT)
	{
		//App->map->MovementCost(position.x - 4, position.y, LEFT)
		if (App->map->MovementCost(position.x - offset_x, position.y, offset_x, offset_y, LEFT) == 0)
		{
			position.x -= 4;
		}

		if (position.x < (previus_position.x - 30))
		{
			state = IDLE;
		}
	}
	if (dir_hit == RIGHT)
	{
		//App->map->MovementCost(position.x + (speed + width), position.y, RIGHT)
		if (App->map->MovementCost(position.x + offset_x, position.y, offset_x, offset_y, RIGHT) == 0)
		{
			position.x += 4;
		}
		if (position.x > (previus_position.x + 30))
		{
			state = IDLE;
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


