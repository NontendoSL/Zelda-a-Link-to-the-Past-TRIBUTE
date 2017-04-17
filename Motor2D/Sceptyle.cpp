#include "Sceptyle.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1GuiElements.h"

Sceptyle::Sceptyle()
{
}

Sceptyle::~Sceptyle()
{
}

bool Sceptyle::Awake(pugi::xml_node &conf )
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
	hp = conf.attribute("hp").as_int(0);
	cooldown = conf.attribute("cooldown").as_int(0);
	attack = conf.attribute("attack").as_int(0);
	speed = conf.attribute("speed").as_int(0);
	name = conf.attribute("name").as_string("");
	position.x = conf.attribute("pos_x").as_int(0);
	position.y = conf.attribute("pos_y").as_int(0);
	active = conf.attribute("active").as_bool(false);
	sp_damage = conf.attribute("special_attack").as_int(0);
	return true;
}

bool Sceptyle::Start()
{
	pokemon_player = true;
	state = IDLE;
	scale = App->win->GetScale();
	offset_x = 7;
	timetoplay = SDL_GetTicks();
	movable = true;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 15, 15 }, COLLIDER_POKEMON, this);
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	reset_run = true;
	sp_attacking = false;
	range = { 90,0 };
	sp_speed = 5;
	return true;
}

bool Sceptyle::Update(float dt)
{
	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME)
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
			case HIT:
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
			case IDLE:
			{
				Idle_IA();
				break;
			}
			case WALKING:
			{
				Walking_IA();
				/*if (target != nullptr && orient_time.ReadSec() >= 2)
				{
					orient_time.Start();
					OrientateTo(target->position);
				}*/
				break;
			}
			case ATTACKING:
			{
				Attack_IA();
				break;
			}
			case HIT:
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

void Sceptyle::Draw()
{
	if (sp_attacking == true)
	{
		ThrowSP();
	}
	if (state == HIT)
	{
		App->anim_manager->Drawing_Manager((ActionState)3, direction, position, 8);
	}
	else
		App->anim_manager->Drawing_Manager(state, direction, position, 8);
}

bool Sceptyle::CleanUp()
{
	return true;
}

void Sceptyle::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		Pokemon* isActive = (Pokemon*)c1->callback;
		Pokemon* isActive_2 = (Pokemon*)c2->callback;
		if (isActive != nullptr && isActive_2 != nullptr)
		{
			if (isActive->active && isActive_2->active)
			{
				if (c1 == collision_attack && c2->type == COLLIDER_POKEMON && getdamage == false)
				{
					if (pokemon_player)
					{
						Pokemon* temp = (Pokemon*)c2->callback;
						temp->knockback_time.Start();
						temp->hp -= attack;
						getdamage = true;
						App->scene->pokecombat->GetDamage(attack, false);
						temp->state = HIT;
						temp->dir_hit = c1->callback->direction;
						temp->prev_position = temp->position;
					}
				}

				if (c1 == sp_attack && c2->type == COLLIDER_POKEMON && getdamage == false)
				{
					if (pokemon_player && c1->callback != c2->callback)
					{
						Pokemon* temp = (Pokemon*)c2->callback;
						temp->hp -= sp_damage;
						getdamage = true;
						App->scene->pokecombat->GetDamage(sp_damage, false);
						temp->state = HIT;
						temp->dir_hit = c1->callback->direction;
						temp->prev_position = temp->position;
					}
				}
			}
		}
	}
}

void Sceptyle::AttackSpecial()
{
	sp_attacking = true;
	sp_direction = direction;
	sp_start = position;
	sp_attack = App->collision->AddCollider({ position.x,position.y, 10, 10 }, COLLIDER_POKEMON_ATTACK, this);
}

void Sceptyle::ThrowSP()
{
	if (sp_attack != nullptr)
	{
		switch (sp_direction)
		{
		case 0:
			App->anim_manager->Drawing_Manager((ActionState)3, UP, { sp_start.x,sp_start.y - range.y }, 10);
			sp_attack->SetPos(sp_start.x, sp_start.y - range.y);
			break;
		case 1:
			App->anim_manager->Drawing_Manager((ActionState)3, UP, { sp_start.x,sp_start.y + range.y }, 10);
			sp_attack->SetPos(sp_start.x, sp_start.y + range.y);
			break;
		case 2:
			App->anim_manager->Drawing_Manager((ActionState)3, UP, { sp_start.x - range.y,sp_start.y - 10 }, 10);
			sp_attack->SetPos(sp_start.x - range.y, sp_start.y - 10);
			break;
		case 3:
			App->anim_manager->Drawing_Manager((ActionState)3, UP, { sp_start.x + range.y,sp_start.y - 10 }, 10);
			sp_attack->SetPos(sp_start.x + range.y, sp_start.y - 10);
			break;
		}
	}
	
	if (range.y >= range.x)
	{
		range.y = 0;
		App->collision->EraseCollider(sp_attack);
		sp_attacking = false;
		getdamage = false;
	}
	else
	{
		range.y += sp_speed;
	}
}

bool Sceptyle::Idle()
{
	//IDLE SCEPTYLE
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		state = WALKING;
	}

	else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_DOWN)
	{
		if (App->scene->pokecombat->cooldown == false)
		{
			AttackSpecial();
			App->scene->pokecombat->cooldown = true;
			App->scene->pokecombat->cdtime.y = App->scene->pokecombat->cdtime.x;
		}

	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_X) == EVENTSTATE::E_DOWN)
	{
		state = ATTACKING;
		current_animation = App->anim_manager->GetAnimation(state, direction, 8); //this number may need to be changed?
		current_animation->Reset();
	}

	else
	{
		state = IDLE;
	}
	return true;
}

bool Sceptyle::Walking()
{
	walking = false;
	Move();

	if (walking == false)
	{
		state = IDLE;
	}

	else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_DOWN)
	{
		if (App->scene->pokecombat->cooldown == false)
		{
			AttackSpecial();
			App->scene->pokecombat->cooldown = true;
			App->scene->pokecombat->cdtime.y = App->scene->pokecombat->cdtime.x;
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_X) == EVENTSTATE::E_DOWN)
	{
		state = ATTACKING;
		current_animation = App->anim_manager->GetAnimation(state, direction, 8); //This number may need to be changed?
		current_animation->Reset();
	}

	else
	{
		state = WALKING;
	}
	return false;
}

bool Sceptyle::Move()
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

bool Sceptyle::Attack()
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
			getdamage = false;
		}
	}
	else
	{
		attacker = true;
		if (direction == UP)
		{
			collision_attack = App->collision->AddCollider({ position.x - 11, position.y - 39, 22, 8 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(8);
		}
		else if (direction == RIGHT)
		{
			collision_attack = App->collision->AddCollider({ position.x + 9, position.y - 28, 8, 22 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(8);
		}
		else if (direction == DOWN)
		{
			collision_attack = App->collision->AddCollider({ position.x - 10, position.y, 22, 8 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(8);
		}
		else if (direction == LEFT)
		{
			collision_attack = App->collision->AddCollider({ position.x - 16, position.y - 28, 8, 22 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(8);
		}
	}
	return true;
}

bool Sceptyle::Idle_IA()
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
				orient_time.Start();
				reset_distance = true;
			}
		}
	}
	return true;
}

bool Sceptyle::Walking_IA()
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

bool Sceptyle::Move_IA()
{
	//App->pathfinding->CreatePath(position, target->Getposition());
	//App->pathfinding->CreatePath(position, target->Getposition());
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

bool Sceptyle::Attack_IA()
{
	return true;
}

bool Sceptyle::Movebyhit()
{
	if (hp <= 0)
	{
		state = DYING;
		return true;
	}

	if (knockback_time.ReadSec() >= 0.2)
	{
		state = IDLE;
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
