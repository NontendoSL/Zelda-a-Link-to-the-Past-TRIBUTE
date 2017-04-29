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

	cooldown = conf.attribute("cooldown").as_int(0);
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
	offset_x = 7;
	offset_y = 7;
	movable = true;
	collision_feet = App->collision->AddCollider({ position.x, position.y, 15, 15 }, COLLIDER_POKEMON, this);
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	reset_run = true;

	//Get the animations
	animation = *App->anim_manager->GetAnimStruct(GEODUDE);

	return true;
}

bool Geodude::Update(float dt)
{
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
			Walking();
			break;
		}
		case P_ATTACKING:
		{
			Attack();
			break;
		}
		case P_HIT:
		{
			Movebyhit();
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

void Geodude::Draw()
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

bool Geodude::Move()
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

	App->entity_elements->DeletePokemon(this);
	return true;
}

bool Geodude::Movebyhit()
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
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - 1.5, collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			position.y -= 1.5;
		}
	}
	else if (dir_hit == DOWN)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + 1.5, collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += 1.5;
		}
	}
	else if (dir_hit == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - 1.5, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= 1.5;
		}
	}
	else if (dir_hit == RIGHT)
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + 1.5, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += 1.5;
		}
	}

	return true;
}

void Geodude::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr && state != P_DYING)
	{
		//TODO JORDI
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
				hp--;
			}
		}

		if (c1 == collision_feet && c2->type == COLLIDER_PLAYER && c2->callback != nullptr)
		{
			if (((Player*)c2->callback)->GetState() != L_HIT)
			{
				if (((Player*)c2->callback)->GetState() != L_HOOKTHROWN && state != L_HIT)
				{
					state = P_ATTACKING;
					anim_state = P_ATTACKING;
					animation.anim[anim_state].ResetAnimations();
					Orientate();
				}
			}
		}
		/*if (c1 == collision_feet && c2->type == COLLIDER_POKEMON && c2->callback != nullptr)
		{
			if (state != HOOKTHROWN)//TODO MED -> change this (we will have golbats int the future)
			{
				if (c2->callback->name == "Golem" && c2->callback->state == STATIC)
				{
					//Not dammage
				}
				else if (state != STATIC)
				{
					if (hurt == false)
					{
						timer.Start();
						hurt = true;
						if (hp_hearts.y > 0)
						{
							hp_hearts.y--;
						}

						if (direction == UP)
						{
							if (App->map->MovementCost(position.x, position.y + 15, offset_x, offset_y, DOWN) == 0)
							{
								position.y += 15;
								if (Camera_inside(iPoint(0, 15)))
									App->render->camera.y -= 15;
							}
						}
						if (direction == DOWN)
						{
							if (App->map->MovementCost(position.x, position.y - 15, offset_x, offset_y, UP) == 0)
							{
								position.y -= 15;
								if (Camera_inside(iPoint(0, 15)))
									App->render->camera.y += 15;
							}
						}
						if (direction == LEFT)
						{
							if (App->map->MovementCost(position.x + 15, position.y, offset_x, offset_y, RIGHT) == 0)
							{
								position.x += 15;
								if (Camera_inside(iPoint(15, 0)))
									App->render->camera.x -= 15;
							}
						}
						if (direction == RIGHT)
						{
							if (App->map->MovementCost(position.x - 15, position.y, offset_x, offset_y, LEFT) == 0)
							{
								position.x -= 15;
								if (Camera_inside(iPoint(15, 0)))
									App->render->camera.x += 15;
							}
						}
					}
					else
					{
						if (timer.ReadSec() >= 1)
						{
							hurt = false;
						}
					}
				}
			}
		}*/
	}
}