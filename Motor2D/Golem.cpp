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

Golem::Golem()
{
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
	state = STATIC;
	scale = App->win->GetScale();
	offset_x = 8;
	offset_y = 17;
	gamestate = TIMETOPLAY;
	movable = true;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 15, 15 }, COLLIDER_POKEMON, this);
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	reset_run = true;

	//Modify Meta
	iPoint temp = App->map->WorldToMap(position.x, position.y);
	App->map->EditCost(temp.x, temp.y, App->map->data.tilesets[1]->firstgid + 1);
	App->map->EditCost(temp.x - 1, temp.y, App->map->data.tilesets[1]->firstgid + 1);
	App->map->EditCost(temp.x, temp.y - 1, App->map->data.tilesets[1]->firstgid + 1);
	App->map->EditCost(temp.x - 1, temp.y - 1, App->map->data.tilesets[1]->firstgid + 1);

	//Get the animations
	animation = *App->anim_manager->GetAnimStruct(5); //id 5 = Golem
	return true;
}

bool Golem::Update()
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
			Hit();
			break;
		}
		case STATIC:
		{
			break;
		}
		/* This case shoul be added, it reffers when abandoning rock stage */
		case AWAKENING:
		{
			if (mode_stone)
			{
				mode_stone = false;
				timetoplay = SDL_GetTicks();
			}
			if (mode_stone == false && SDL_GetTicks() - timetoplay > 500)
			{
				state = IDLE;
			}
			break;
		}
		
		case DYING:
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

void Golem::Draw()
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
	case STATIC:
		id = 3;
		break;
	case AWAKENING:
		id = 4;
		break;
	case HIT:
		id = 5;
		break;
	case DYING:
		id = 5;
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

bool Golem::CleanUp()
{
	return true;
}

void Golem::AddItem(Item* item)
{
	item_inside = item;
	item->canBlit = false;
}

void Golem::Drop_item()
{
	item_inside->canBlit = true;
	item_inside->position.x = position.x;
	item_inside->position.y = position.y;
	item_inside = NULL;
}

bool Golem::Idle()
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

bool Golem::Walking()
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

bool Golem::Move()
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

bool Golem::Hit()
{
	if(hp <= 0)
	{
		state = DYING;
		return true;
	}

	if (hurt_timer.ReadSec() >= 0.2)
	{
		state = IDLE;
		return true;
	}
	return true;
}

bool Golem::Attack()
{
	return true;
}

bool Golem::Death()
{
	if (App->scene->player->bombmanager != nullptr)
	{
		iPoint pos;
		pos.create(position.x - offset_x, position.y - offset_y);
		App->scene->items.push_back(App->entity_elements->CreateItem(2, pos));
	}

	App->entity_elements->DeletePokemon(this);
	return true;
}

void Golem::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		if (c1 == collision_feet && c2->type == COLLIDER_BOMB)
		{
			if (state == STATIC)
			{
				state = AWAKENING;
				iPoint temp = App->map->WorldToMap(position.x, position.y);
				App->map->EditCost(temp.x, temp.y, 0);
				App->map->EditCost(temp.x - 1, temp.y, 0);
				App->map->EditCost(temp.x, temp.y - 1, 0);
				App->map->EditCost(temp.x - 1, temp.y - 1, 0);

			}
		}
		if(c1 == collision_feet && c2 == App->scene->player->GetCollisionAttack() && state != HIT && state != STATIC)
		{
			animation.anim[5].ResetAnimations();
			hurt_timer.Start();
			state = HIT;
			hp--;
		}
	}
}