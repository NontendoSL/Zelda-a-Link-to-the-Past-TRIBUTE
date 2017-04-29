#include "Blaziken.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1GuiElements.h"



Blaziken::Blaziken()
{
}

Blaziken::~Blaziken()
{
}

bool Blaziken::Awake(pugi::xml_node &conf)
{
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
	hp = conf.attribute("hp").as_int(0);
	attack = conf.attribute("attack").as_int(0);
	speed = conf.attribute("speed").as_int(0);
	name = conf.attribute("name").as_string("");
	position.x = conf.attribute("pos_x").as_int(0);
	position.y = conf.attribute("pos_y").as_int(0);
	active = conf.attribute("active").as_bool(false);
	sp_damage = conf.attribute("special_attack").as_int(0);
	return true;
}

bool Blaziken::Start()
{
	pokemon_player = true;
	state = P_IDLE;
	anim_state = P_IDLE;
	scale = App->win->GetScale();
	offset_x = 7;
	offset_y = 17;
	timetoplay = SDL_GetTicks();
	movable = true;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 15, 15 }, COLLIDER_POKEMON, this);
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	sp_attacking = false;
	reset_run = true;
	return true;
}

bool Blaziken::Update(float dt)
{
	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME)
	{
		if (pokemon_player)
		{
			//pokemon controlled by player
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
			case P_SPECIAL:
			{
				Attack();
				break;
			}
			case P_HIT:
			{
				Movebyhit();
				break;
			}
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
			case P_IDLE:
			{
				Idle_IA();
				break;
			}
			case P_WALKING:
			{
				Walking_IA();
				/*if (target != nullptr && orient_time.ReadSec() >= 2)
				{
					orient_time.Start();
					OrientateTo(target->position);
				}*/
				break;
			}
			case P_ATTACKING:
			{
				//Attack_IA();
				break;
			}
			case P_HIT:
			{
				Movebyhit();
				break;
			}
			default:
			{
				break;
			}

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

void Blaziken::Draw()
{
	App->anim_manager->Drawing_Manager(anim_state, direction, position, BLAZIKEN);
}

bool Blaziken::CleanUp()
{
	return true;
}

void Blaziken::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		Pokemon* isActive = (Pokemon*)c1->callback;
		Pokemon* isActive_2 = (Pokemon*)c2->callback;
		if (isActive != nullptr && isActive_2 != nullptr)
		{
			if (isActive->active && isActive_2->active)
			{

				if (c1 == sp_attack && c2->type == COLLIDER_POKEMON && getdamage == false && pokemon_player)
				{
					Pokemon* temp = (Pokemon*)c2->callback;
					temp->knockback_time.Start();
					temp->hp -= sp_damage;
					getdamage = true;
					App->scene->pokecombat->GetDamage(sp_damage, false);
					temp->SetState(P_HIT);
					temp->SetAnimState(P_DYING);
					temp->dir_hit = c1->callback->direction;
					temp->prev_position = temp->position;
				}

				if (c1 == collision_attack && c2->type == COLLIDER_POKEMON && getdamage == false)
				{
					if (isActive->pokemon_player)
					{
						Pokemon* temp = (Pokemon*)c2->callback;
						temp->knockback_time.Start();
						temp->hp -= attack;
						getdamage = true;
						App->scene->pokecombat->GetDamage(attack, false);
						temp->SetState(P_HIT);
						temp->SetAnimState(P_DYING);
						temp->dir_hit = c1->callback->direction;
						temp->prev_position = temp->position;
					}
				}
			}
		}
	}
}

bool Blaziken::Idle()
{
	//IDLE BLAZIKEN
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		state = P_WALKING;
		anim_state = P_WALKING;
	}

	else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_DOWN)
	{
		if (App->scene->pokecombat->cooldown == false)
		{
			App->scene->pokecombat->cooldown = true;
			state = P_SPECIAL;
			anim_state = P_SPECIAL;
			current_animation = App->anim_manager->GetAnimation(state, direction, BLAZIKEN);
			current_animation->Reset();
			sp_attacking = true;
			App->scene->pokecombat->cdtime.y = App->scene->pokecombat->cdtime.x;
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_X) == EVENTSTATE::E_DOWN)
	{
		state = P_ATTACKING;
		anim_state = P_ATTACKING;
		current_animation = App->anim_manager->GetAnimation(state, direction, BLAZIKEN);
		current_animation->Reset();
	}

	else
	{
		state = P_IDLE;
		anim_state = P_IDLE;
	}
	return true;
}

bool Blaziken::Walking()
{
	walking = false;
	Move();

	if (walking == false)
	{
		state = P_IDLE;
		anim_state = P_IDLE;
	}

	else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_DOWN)
	{
		if (App->scene->pokecombat->cooldown == false)
		{
			App->scene->pokecombat->cooldown = true;
			state = P_SPECIAL;
			anim_state = P_SPECIAL;
			current_animation = App->anim_manager->GetAnimation(state, direction, BLAZIKEN);
			current_animation->Reset();
			sp_attacking = true;
			App->scene->pokecombat->cdtime.y = App->scene->pokecombat->cdtime.x;
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_X) == EVENTSTATE::E_DOWN)
	{
		state = P_ATTACKING;
		anim_state = P_ATTACKING;
		current_animation = App->anim_manager->GetAnimation(state, direction, BLAZIKEN);
		current_animation->Reset();
	}

	else
	{
		state = P_WALKING;
		anim_state = P_WALKING;
	}
	return false;
}

bool Blaziken::Move()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT)
	{
		direction = LEFT;
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

bool Blaziken::Attack()
{
	if (attacker)
	{
		if (current_animation->Finished())
		{
			if (sp_attacking)
			{
				App->collision->EraseCollider(sp_attack);
				sp_attacking = false;
			}
			else
			{
				App->collision->EraseCollider(collision_attack);

			}
			attacker = false;
			current_animation->Reset();
			current_animation = nullptr;
			state = P_IDLE;
			anim_state = P_IDLE;
			if (getdamage)
			{
				getdamage = false;
			}
		}
	}
	else
	{
		attacker = true;
		if (sp_attacking)
		{
			if (direction == UP)
			{
				sp_attack = App->collision->AddCollider({ position.x - 11, position.y - 35, 22, 8 }, COLLIDER_POKEMON_ATTACK, this);
			}
			else if (direction == RIGHT)
			{
				sp_attack = App->collision->AddCollider({ position.x + 15, position.y - 26, 8, 22 }, COLLIDER_POKEMON_ATTACK, this);
			}
			else if (direction == DOWN)
			{
				sp_attack = App->collision->AddCollider({ position.x - 10, position.y, 22, 8 }, COLLIDER_POKEMON_ATTACK, this);
			}
			else if (direction == LEFT)
			{
				sp_attack = App->collision->AddCollider({ position.x - 23, position.y - 26, 8, 22 }, COLLIDER_POKEMON_ATTACK, this);
			}
			App->audio->PlayFx(7);
		}
		else
		{
			if (direction == UP)
			{
				collision_attack = App->collision->AddCollider({ position.x - 11, position.y - 35, 22, 8 }, COLLIDER_POKEMON_ATTACK, this);
			}
			else if (direction == RIGHT)
			{
				collision_attack = App->collision->AddCollider({ position.x + 15, position.y - 26, 8, 22 }, COLLIDER_POKEMON_ATTACK, this);
			}
			else if (direction == DOWN)
			{
				collision_attack = App->collision->AddCollider({ position.x - 10, position.y, 22, 8 }, COLLIDER_POKEMON_ATTACK, this);
			}
			else if (direction == LEFT)
			{
				collision_attack = App->collision->AddCollider({ position.x - 23, position.y - 26, 8, 22 }, COLLIDER_POKEMON_ATTACK, this);
			}
			App->audio->PlayFx(9);
		}

	}
	return true;
}

bool Blaziken::Idle_IA()
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
				state = P_WALKING;
				anim_state = P_WALKING;
				orient_time.Start();
				reset_distance = true;
			}
		}
	}
	return true;
}

bool Blaziken::Walking_IA()
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

bool Blaziken::Move_IA()
{
	//App->pathfinding->CreatePath(position, target->Getposition());
	if (direction == LEFT)
	{
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

bool Blaziken::Attack_IA()
{

	return true;
}

bool Blaziken::Movebyhit()
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
	/*if (position.x > (prev_position.x + 65) ||
	position.x < (prev_position.x + 65) ||
	position.y >(prev_position.y + 65) ||
	position.y < (prev_position.y + 65))
	{
	state = IDLE;
	}*/
	return true;
}
