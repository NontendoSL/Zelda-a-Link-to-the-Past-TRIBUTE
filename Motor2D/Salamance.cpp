#include "Salamance.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1Audio.h"
#include "j1GuiElements.h"
#include "CombatManager.h"

Salamance::Salamance()
{
}

Salamance::~Salamance()
{
}


bool Salamance::Awake(pugi::xml_node &conf)
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

bool Salamance::Start()
{
	state = PC_IDLE;
	anim_state = PC_IDLE;
	scale = App->win->GetScale();
	offset_x = 7;
	offset_y = 17;
	timetoplay = SDL_GetTicks();
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 15, 15 }, COLLIDER_POKEMON, this);
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	sp_attacking = false;
	reset_run = true;
	return true;
}

bool Salamance::Update(float dt)
{
	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME)
	{
		//pokemon controlled by player
		switch (state)
		{
		case PC_IDLE:
		{
			Idle();
			break;
		}
		case PC_WALKING:
		{
			Walking(dt);
			break;
		}
		case PC_CHASING:
		{
			Orientate();
			Chasing(dt);
		}
		case PC_ATTACKING:
		{
			Attack();
			break;
		}
		case PC_SPECIAL:
		{
			Special_Attack();
			break;
		}
		case PC_HIT:
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
	/*	**Only the special attack is launch.**
	if (drawThrowSP)
	{
	ThrowSP();
	}*/

	if (CheckPlayerPos() < 30 && state == PC_WALKING)
	{
		Orientate();
		state = PC_CHASING;
	}

	if (CheckPlayerPos() < 6 && (state == PC_WALKING || state == PC_CHASING))
	{
		Orientate();
		state = PC_ATTACKING;
	}

	//Collision follow the player
	collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
	return true;
}

void Salamance::Draw()
{
	/*if (drawThrowSP)  **Only the special attack is launch.**
	{
	if (sp_attack != nullptr)
	{
	switch (sp_direction)
	{
	case 0:
	App->anim_manager->Drawing_Manager(LEAF, (Direction)0, { sp_start.x,sp_start.y - range.y }, PARTICLES);
	sp_attack->SetPos(sp_start.x, sp_start.y - range.y);
	break;
	case 1:
	App->anim_manager->Drawing_Manager(LEAF, (Direction)0, { sp_start.x,sp_start.y + range.y }, PARTICLES);
	sp_attack->SetPos(sp_start.x, sp_start.y + range.y);
	break;
	case 2:
	App->anim_manager->Drawing_Manager(LEAF, (Direction)0, { sp_start.x - range.y,sp_start.y - 10 }, PARTICLES);
	sp_attack->SetPos(sp_start.x - range.y, sp_start.y - 10);
	break;
	case 3:
	App->anim_manager->Drawing_Manager(LEAF, (Direction)0, { sp_start.x + range.y,sp_start.y - 10 }, PARTICLES);
	sp_attack->SetPos(sp_start.x + range.y, sp_start.y - 10);
	break;
	}
	}
	}*/
	App->anim_manager->Drawing_Manager(anim_state, direction, position, SALAMENCE);
}

bool Salamance::CleanUp()
{
	return true;
}

void Salamance::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		PokemonCombat* isActive = (PokemonCombat*)c1->callback;
		PokemonCombat* isActive_2 = (PokemonCombat*)c2->callback;
		if (isActive != nullptr && isActive_2 != nullptr)
		{
			if (isActive->active && isActive_2->active)
			{
				if (c1 == sp_attack && c2->type == COLLIDER_POKEMON && getdamage == false)
				{
					PokemonCombat* temp = (PokemonCombat*)c2->callback;
					temp->knockback_time.Start();
					temp->hp -= sp_damage;
					getdamage = true;
					App->scene->pokecombat->GetDamage(sp_damage, false);
					temp->SetState(PC_HIT);
					temp->SetAnimState(PC_HIT);
					temp->dir_hit = c1->callback->direction;
					temp->prev_position = temp->position;
				}

				if (c1 == collision_attack && c2->type == COLLIDER_POKEMON && getdamage == false)
				{
					PokemonCombat* temp = (PokemonCombat*)c2->callback;
					temp->knockback_time.Start();
					temp->hp -= attack;
					getdamage = true;
					App->scene->pokecombat->GetDamage(attack, false);
					temp->SetState(PC_HIT);
					temp->SetAnimState(PC_HIT);
					temp->dir_hit = c1->callback->direction;
					temp->prev_position = temp->position;
				}
			}
		}
	}
}

bool Salamance::Idle()
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
			state = PC_WALKING;
			anim_state = PC_WALKING;
			orient_time.Start();
			reset_distance = true;
		}
	}
	return true;
}

bool Salamance::Walking(float dt)
{
	walking = false;
	if (reset_distance)
	{
		distance = rand() % 60 + 20;
		dis_moved = 0;
		reset_distance = false;
	}
	if (canmove % 2 == 0)
	{
		Move(dt);
	}


	if (dis_moved >= distance)
	{
		walking = false;
		reset_run = true;
	}


	if (walking == false)
	{
		state = PC_IDLE;
		anim_state = PC_IDLE;
	}

	else
	{
		state = PC_WALKING;
		anim_state = PC_WALKING;
	}
	return true;
}

bool Salamance::Move(float dt)
{
	//App->pathfinding->CreatePath(position, target->Getposition());
	if (direction == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - speed, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= ceil(speed * dt);
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
			position.x += ceil(speed * dt);
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
			position.y -= ceil(speed * dt);
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
			position.y += ceil(speed * dt);
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

bool Salamance::Attack()
{
	if (attacker)
	{
		if (current_animation->Finished())
		{
			App->collision->EraseCollider(collision_attack);
			attacker = false;
			current_animation->Reset();
			current_animation = nullptr;
			state = PC_IDLE;
			anim_state = PC_IDLE;
			getdamage = false;
		}
	}
	else
	{
		attacker = true;
		if (direction == UP)
		{
			collision_attack = App->collision->AddCollider({ position.x - 11, position.y - 35, 22, 8 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(10);
		}
		else if (direction == RIGHT)
		{
			collision_attack = App->collision->AddCollider({ position.x + 12, position.y - 26, 8, 22 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(10);
		}
		else if (direction == DOWN)
		{
			collision_attack = App->collision->AddCollider({ position.x - 10, position.y - 4, 22, 8 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(10);
		}
		else if (direction == LEFT)
		{
			collision_attack = App->collision->AddCollider({ position.x - 20, position.y - 26, 8, 22 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(10);
		}
	}
	return true;
}

/*void Dusclops::ThrowSP() **Only the special attack is launch.**
{

}*/

void Salamance::Special_Attack()
{

}

bool Salamance::Chasing(float dt)
{
	walking = false;
	if (reset_distance)
	{
		distance = rand() % 75 + 35;
		dis_moved = 0;
		reset_distance = false;
	}
	Move(dt);


	if (dis_moved >= distance)
	{
		walking = false;
		reset_run = true;
	}


	if (walking == false)
	{
		state = PC_IDLE;
		anim_state = PC_IDLE;
	}
	else
	{
		state = PC_WALKING;
		anim_state = PC_WALKING;
	}
	return true;
}

bool Salamance::Movebyhit()
{
	if (hp <= 0)
	{
		state = PC_DYING;
		anim_state = PC_DYING;
		return true;
	}

	if (knockback_time.ReadSec() >= 0.2)
	{
		state = PC_IDLE;
		anim_state = PC_IDLE;
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

int Salamance::CheckPlayerPos()
{
	return App->combatmanager->pokemon_active_link->position.DistanceTo(position);
}