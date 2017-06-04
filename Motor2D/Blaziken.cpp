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
	type = CREATURE;
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
	defense = conf.attribute("defense").as_int(0);
	return true;
}

bool Blaziken::Start()
{
	state = PC_IDLE;
	anim_state = PC_IDLE;
	scale = App->win->GetScale();
	offset_x = 7;
	offset_y = 17;
	timetoplay = SDL_GetTicks();
	movable = true;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 15, 15 }, COLLIDER_POKECOMBAT, this);
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	sp_attacking = false;
	reset_run = true;
	return true;
}

bool Blaziken::Update(float dt)
{
	BROFILER_CATEGORY("Upcate_Collision", Profiler::Color::Lime);
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
			Movebyhit(4);
			break;
		}
		case PC_STUNNED:
		{
			Stunned();
			break;
		}
		case PC_COLLISION:
		{
			Movebyhit(1);
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

	if (App->combatmanager->pokemon_active_trainer->name != "DUSCLOPS")
	{
		dusclops_special = false;
	}
	if (dusclops_special == false && App->combatmanager->pokemon_active_trainer->name == "DUSCLOPS" && App->combatmanager->pokemon_active_trainer->Special_inUse())
	{
		dusclops_special = true;
		autoAttack.Start();
	}
	if (dusclops_special)
	{
		if (autoAttack.ReadSec() > 0.5)
		{
			autoAttack.Start();
			hp -= 5;
			App->scene->pokecombat->GetDamage(1, true);
		}
	}

	//Collision follow the player
	collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
	return true;
}

void Blaziken::Draw()
{
	BROFILER_CATEGORY("Draw_Blaziquen", Profiler::Color::HotPink);
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
		PokemonCombat* pokemon_1 = (PokemonCombat*)c1->callback;
		PokemonCombat* pokemon_2 = (PokemonCombat*)c2->callback;
		if (pokemon_1 != nullptr && pokemon_2 != nullptr)
		{
			if (pokemon_1->active && pokemon_2->active)
			{
				if (c1 == sp_attack && c2->type == COLLIDER_POKECOMBAT && getdamage == false && pokemon_2->GetState() != PC_SPECIAL)
				{
					pokemon_2->knockback_time.Start();
					pokemon_2->hp -= sp_damage;
					getdamage = true;
					App->scene->pokecombat->GetDamage(sp_damage, false);
					pokemon_2->SetState(PC_HIT);
					pokemon_2->SetAnimState(PC_HIT);
					pokemon_2->dir_hit = c1->callback->direction;
					pokemon_2->prev_position = pokemon_2->position;
				}

				if (c1 == collision_attack && c2->type == COLLIDER_POKECOMBAT && getdamage == false && pokemon_2->GetState() != PC_SPECIAL)
				{
					pokemon_2->knockback_time.Start();
					pokemon_2->hp -= attack;
					getdamage = true;
					App->scene->pokecombat->GetDamage(attack, false);
					pokemon_2->SetState(PC_HIT);
					pokemon_2->SetAnimState(PC_HIT);
					pokemon_2->dir_hit = c1->callback->direction;
					pokemon_2->prev_position = pokemon_2->position;
				}

				if (c1 == collision_feet && c2->type == COLLIDER_POKECOMBAT)
				{
					state = PC_COLLISION;
					anim_state = PC_IDLE;
					dir_hit = pokemon_2->direction;
					knockback_time.Start();

					if (pokemon_2->GetState() != PC_SPECIAL)
					{
						pokemon_2->SetState(PC_COLLISION);
						pokemon_2->SetAnimState(PC_IDLE);
						if (pokemon_2->direction == UP)
						{
							pokemon_2->dir_hit = DOWN;
						}
						else if (pokemon_2->direction == DOWN)
						{
							pokemon_2->dir_hit = UP;
						}
						else if (pokemon_2->direction == RIGHT)
						{
							pokemon_2->dir_hit = LEFT;
						}
						else
						{
							pokemon_2->dir_hit = RIGHT;
						}
						pokemon_2->knockback_time.Start();
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
		state = PC_WALKING;
		anim_state = PC_WALKING;
	}

	else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_DOWN)
	{
		if (App->scene->pokecombat->cooldown == false)
		{
			App->scene->pokecombat->cooldown = true;
			state = PC_SPECIAL;
			anim_state = PC_SPECIAL;
			current_animation = App->anim_manager->GetAnimation(anim_state, direction, BLAZIKEN);
			current_animation->Reset();
			App->scene->pokecombat->cdtime.y = App->scene->pokecombat->cdtime.x;
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_X) == EVENTSTATE::E_DOWN)
	{
		state = PC_ATTACKING;
		anim_state = PC_ATTACKING;
		current_animation = App->anim_manager->GetAnimation(anim_state, direction, BLAZIKEN);
		current_animation->Reset();
	}

	else
	{
		state = PC_IDLE;
		anim_state = PC_IDLE;
	}
	return true;
}

bool Blaziken::Walking(float dt)
{
	walking = false;
	Move(dt);

	if (walking == false)
	{
		state = PC_IDLE;
		anim_state = PC_IDLE;
	}

	else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_DOWN)
	{
		if (App->scene->pokecombat->cooldown == false)
		{
			App->scene->pokecombat->cooldown = true;
			state = PC_SPECIAL;
			anim_state = PC_SPECIAL;
			current_animation = App->anim_manager->GetAnimation(anim_state, direction, BLAZIKEN);
			current_animation->Reset();
			App->scene->pokecombat->cdtime.y = App->scene->pokecombat->cdtime.x;
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_X) == EVENTSTATE::E_DOWN)
	{
		state = PC_ATTACKING;
		anim_state = PC_ATTACKING;
		current_animation = App->anim_manager->GetAnimation(anim_state, direction, BLAZIKEN);
		current_animation->Reset();
	}

	else
	{
		state = PC_WALKING;
		anim_state = PC_WALKING;
	}
	return false;
}

bool Blaziken::Move(float dt)
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
			App->collision->EraseCollider(collision_attack);
			collision_attack = nullptr;
			attacker = false;
			current_animation->Reset();
			current_animation = nullptr;
			state = PC_IDLE;
			anim_state = PC_IDLE;
			if (getdamage)
			{
				getdamage = false;
			}
		}
	}
	else
	{
		attacker = true;
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
	return true;
}

bool Blaziken::Special_Attack()
{
	if (sp_attacking)
	{
		if (current_animation->Finished())
		{
			App->collision->EraseCollider(sp_attack);
			sp_attacking = false;
			current_animation->Reset();
			current_animation = nullptr;
			state = PC_IDLE;
			anim_state = PC_IDLE;
			if (getdamage)
			{
				getdamage = false;
			}
		}
	}
	else
	{
		sp_attacking = true;
		if (direction == UP)
		{
			sp_attack = App->collision->AddCollider({ position.x - 11, position.y - 35, 22, 8 }, COLLIDER_POKEMON_SPECIAL_ATTACK, this);
		}
		else if (direction == RIGHT)
		{
			sp_attack = App->collision->AddCollider({ position.x + 15, position.y - 26, 8, 22 }, COLLIDER_POKEMON_SPECIAL_ATTACK, this);
		}
		else if (direction == DOWN)
		{
			sp_attack = App->collision->AddCollider({ position.x - 10, position.y, 22, 8 }, COLLIDER_POKEMON_SPECIAL_ATTACK, this);
		}
		else if (direction == LEFT)
		{
			sp_attack = App->collision->AddCollider({ position.x - 23, position.y - 26, 8, 22 }, COLLIDER_POKEMON_SPECIAL_ATTACK, this);
		}
		App->audio->PlayFx(7);
	}

	return true;
}

void Blaziken::Stunned()
{
	if (hp <= 0)
	{
		state = PC_DYING;
		anim_state = PC_DYING;
	}
	else
	{
		if (time_stunned.ReadSec() >= 1)
		{
			state = PC_IDLE;
			anim_state = PC_IDLE;
		}

		if (dir_hit == UP)
		{
			if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - 4, collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
			{
				position.y -= 2;
			}
		}
		else if (dir_hit == DOWN)
		{
			if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + 4, collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
			{
				position.y += 2;
			}
		}
		else if (dir_hit == LEFT)
		{
			if (App->map->MovementCost(collision_feet->rect.x - 4, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
			{
				position.x -= 2;
			}
		}
		else if (dir_hit == RIGHT)
		{
			if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + 4, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
			{
				position.x += 2;
			}
		}
	}
}

bool Blaziken::Movebyhit(int speed)
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
			position.y -= speed;
		}
	}
	else if (dir_hit == DOWN)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + 4, collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += speed;
		}
	}
	else if (dir_hit == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - 4, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= speed;
		}
	}
	else if (dir_hit == RIGHT)
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + 4, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += speed;
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
