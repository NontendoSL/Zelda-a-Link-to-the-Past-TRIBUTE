#include "Swampert.h"
#include "j1App.h"
#include "j1Pathfinding.h"

Swampert::Swampert()
{
}

Swampert::~Swampert()
{
}

bool Swampert::Awake(pugi::xml_node &, uint)
{
	return false;
}

bool Swampert::Start()
{
	return false;
}

bool Swampert::Update(float dt)
{
	// STATE MACHINE ------------------
	if (gamestate == INGAME)
	{
		if (pokemon_player)
		{
			//pokemon controlled by player
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
			/*
			case SPECIALATTACK:
			{
				SpecialAttack();
				break;
			}
			*/
			default:
			{
				break;
			}

			}
		}
		else
		{
			//Pokemon IA
			switch (state)
			{
			case IDLE:
			{
				Idle_IA();
				break;
			}
			case WALKING:
			{
				Walking_IA();
				break;
			}
			case ATTACKING:
			{
				Attack_IA();
				break;
			}
			/*
			case SPECIALATTACK:
			{
				SpecialAttack();
				break;
			}
			*/
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

	//Collision follow the player
	collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
	return true;
}

void Swampert::Draw()
{
}

bool Swampert::CleanUp()
{
	return true;
}

void Swampert::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		if (c1 == collision_feet && c2->type == COLLIDER_ENEMY)
		{

		}
	}
}

bool Swampert::Idle()
{
	//IDLE SCEPTYLE
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		state = WALKING;
		CheckOrientation();
	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		state = ATTACKING;
		current_animation = App->anim_manager->GetAnimation(state, direction, 1); //this number may need to be changed?
		current_animation->Reset();
	}

	else
	{
		state = IDLE;
	}
	return true;
}

bool Swampert::Walking()
{
	walking = false;
	Move();

	if (walking == false)
	{
		state = IDLE;
	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		state = ATTACKING;
		current_animation = App->anim_manager->GetAnimation(state, direction, 1); //This number may need to be changed?
		current_animation->Reset();
	}

	else
	{
		state = WALKING;
	}
	return false;
}

bool Swampert::Move()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT)
	{
		direction = LEFT;
		//int temp = App->map->MovementCost(position.x - speed, position.y, LEFT);
		int temp = App->map->MovementCost(collision_feet->rect.x - speed, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT);
		if (temp == 0)
		{
			position.x -= speed;
		}

		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT)
	{
		direction = DOWN;
		//int temp = App->map->MovementCost(position.x, position.y + (speed + height), DOWN);
		int temp = App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + speed, collision_feet->rect.w, collision_feet->rect.h, DOWN);
		if (temp == 0)
		{
			position.y += speed;
		}

		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT)
	{
		direction = RIGHT;
		//int temp = App->map->MovementCost(position.x + (speed + width), position.y, RIGHT);
		int temp = App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + speed, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT);
		if (temp == 0)
		{
			position.x += speed;
		}
		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		direction = UP;
		//int temp = App->map->MovementCost(position.x, position.y - speed, UP);
		int temp = App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - speed, collision_feet->rect.w, collision_feet->rect.h, UP);
		if (temp == 0)
		{
			position.y -= speed;
		}
		walking = true;
	}
	return walking;
}

bool Swampert::Attack()
{
	if (attacker)
	{
		if (current_animation->Finished())
		{
			App->collision->EraseCollider(collision_attack);
			attacker = false;
			current_animation->Reset();
			current_animation = nullptr;
			state = IDLE;
		}
	}
	else
	{
		attacker = true;
		if (direction == UP)
		{
			collision_attack = App->collision->AddCollider({ position.x, position.y, 8, 20 }, COLLIDER_PLAYER, this);
		}
		else if (direction == RIGHT)
		{
			collision_attack = App->collision->AddCollider({ position.x, position.y, 20, 8 }, COLLIDER_PLAYER, this);
		}
		else if (direction == DOWN)
		{
			collision_attack = App->collision->AddCollider({ position.x, position.y, 8, 20 }, COLLIDER_PLAYER, this);
		}
		else if (direction == LEFT)
		{
			collision_attack = App->collision->AddCollider({ position.x, position.y, 20, 8 }, COLLIDER_PLAYER, this);
		}
	}
	return true;
}

bool Swampert::Idle_IA()
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
			if (SDL_GetTicks() - timetorun > 200)
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

bool Swampert::Walking_IA()
{
	walking = false;
	if (reset_distance)
	{
		distance = rand() % 60 + 20;
		dis_moved = 0;
		reset_distance = false;
	}
	Move_IA();

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

bool Swampert::Move_IA()
{
	//App->pathfinding->CreatePath(position, target->Getposition());
	return true;
}

bool Swampert::Attack_IA()
{
	return true;
}

bool Swampert::CheckOrientation()
{
	return true;
}

void Swampert::OnInputCallback(INPUTEVENT, EVENTSTATE)
{
}