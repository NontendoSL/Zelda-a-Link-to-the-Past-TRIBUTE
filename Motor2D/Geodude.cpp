#include "Geodude.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "p2Defs.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Item.h"
#include "j1Collision.h"
#include "j1EntityElementsScene.h"

Geodude::Geodude()
{
}

Geodude::~Geodude()
{
}

bool Geodude::Awake(pugi::xml_node &conf, uint id, iPoint pos)
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
	if(mode_stone)
		position = iPoint(pos.x, pos.y);
	else
		position = iPoint(conf.attribute("pos_x").as_int(0), conf.attribute("pos_y").as_int(0));

	return true;
}

bool Geodude::Start()
{
	state = IDLE;
	scale = App->win->GetScale();
	offset_x = 15;
	offset_y = 15;
	gamestate = TIMETOPLAY;
	movable = true;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 15, 15 }, COLLIDER_POKEMON, this);
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	reset_run = true;

	//Get the animations
	animation = *App->anim_manager->GetAnimStruct(4); //id 4 = Geodude

	return true;
}

bool Geodude::Update(float dt)
{
	// STATE MACHINE ------------------
	if (gamestate == INGAME)
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
		case ATTACKING:
		{
			Attack();
			break;
		}
		case HIT:
		{
			Movebyhit();
		}
		case DYING:
		{
			Death();
		}
		default:
		{
			break;
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

	//Collision follow the player
	collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
	return true;
}

void Geodude::Draw()
{
	BROFILER_CATEGORY("Draw_SOLDIER", Profiler::Color::Yellow)
		//App->anim_manager->Drawing_Manager(state, direction, position, 6);
		int id;
	switch (state)
	{
	case IDLE:
		id = 0;
		break;
	case WALKING:
		id = 1;
		break;
	case ATTACKING:
		id = 2;
		break;
	case DYING:
		id = 3;
		break;
	default:
		break;
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

	//DRAW
	App->render->Blit(animation.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);
}

bool Geodude::CleanUp()
{
	return false;
}

void Geodude::AddItem(Item* item)
{
	item_inside = item;
	item->canBlit = false;
}

void Geodude::Drop_item()
{
	item_inside->canBlit = true;
	item_inside->position.x = position.x;
	item_inside->position.y = position.y;
	item_inside = NULL;
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
				state = WALKING;
				reset_distance = true;
			}
		}
	}
	return true;
}

bool Geodude::Walking()
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
}

bool Geodude::Move()
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

bool Geodude::Attack()
{
	return true;
}

bool Geodude::Death()
{
	return true;
}

bool Geodude::Movebyhit()
{
	if (dir_hit == UP)
	{
		//App->map->MovementCost(position.x, position.y - speed, UP)
		if (App->map->MovementCost(position.x, position.y - offset_y, offset_x, offset_y, UP) == 0)
		{
			position.y -= 1.5;
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
			position.y += 1.5;
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
			position.x -= 1.5;
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
			position.x += 1.5;
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