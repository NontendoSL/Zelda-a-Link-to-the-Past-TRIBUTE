#include "Salamance.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1Audio.h"
#include "j1GuiElements.h"
#include "ParticleManager.h"
#include "P_Fire.h"
#include "CombatManager.h"

Salamance::Salamance()
{
	type = CREATURE;
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
	use_cooldown = 0;
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

bool Salamance::Start()
{
	state = PC_IDLE;
	anim_state = PC_IDLE;
	scale = App->win->GetScale();
	offset_x = 13;
	offset_y = 21;
	timetoplay = SDL_GetTicks();
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 26, 19 }, COLLIDER_POKECOMBAT, this);
	timetoplay = SDL_GetTicks();
	reset_distance = false;
	sp_attacking = false;
	reset_run = true;

	change_dir.Start();

	return true;
}

bool Salamance::Update(float dt)
{
	BROFILER_CATEGORY("Update_Collision", Profiler::Color::Null);
	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME)
	{
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
			OrientatePokeLink();
			Chasing(dt);
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

	if (CheckPlayerPos() < VIEWING_DISTANCE && state == PC_WALKING)
	{
		state = PC_CHASING;
	}

	if (CheckPlayerPos() < ATTACK_DISTANCE && (state == PC_WALKING || state == PC_CHASING) && wait_attack.ReadSec() > 0.5)
	{
		OrientatePokeLink();
		state = PC_ATTACKING;
		anim_state = PC_ATTACKING;
		attacker = false;
		current_animation = App->anim_manager->GetAnimation(state, direction, SALAMENCE);
		current_animation->Reset();
	}

	if (use_cooldown == cooldown && CheckPlayerPos() < ATTACK_DISTANCE * 2 && state != PC_SPECIAL)
	{
		state = PC_SPECIAL;
		anim_state = PC_SPECIAL;
		use_cooldown = 0;
		attacker = false;
		current_animation = App->anim_manager->GetAnimation(state, direction, SALAMENCE);
		current_animation->Reset();
	}

	if (use_cooldown < cooldown && state != PC_SPECIAL)
	{
		use_cooldown++;
	}

	//Collision follow the player
	collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
	return true;
}

void Salamance::Draw()
{
	BROFILER_CATEGORY("Draw_Salamance", Profiler::Color::Ivory);
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
		PokemonCombat* pokemon_1 = (PokemonCombat*)c1->callback;
		PokemonCombat* pokemon_2 = (PokemonCombat*)c2->callback;
		if (pokemon_1 != nullptr && pokemon_2 != nullptr)
		{
			if (pokemon_1->active && pokemon_2->active && pokemon_1 != pokemon_2)
			{
				if (c1 == sp_attack && c2->type == COLLIDER_POKECOMBAT && getdamage == false)
				{
					pokemon_2->knockback_time.Start();
					pokemon_2->hp -= sp_damage;
					getdamage = true;
					App->scene->pokecombat->GetDamage(sp_damage, true);
					pokemon_2->SetState(PC_HIT);
					pokemon_2->SetAnimState(PC_HIT);
					pokemon_2->dir_hit = c1->callback->direction;
					pokemon_2->prev_position = pokemon_2->position;
				}

				if (c1 == collision_attack && c2->type == COLLIDER_POKECOMBAT && getdamage == false)
				{
					pokemon_2->knockback_time.Start();
					pokemon_2->hp -= attack;
					getdamage = true;
					App->scene->pokecombat->GetDamage(attack, true);
					pokemon_2->SetState(PC_HIT);
					pokemon_2->SetAnimState(PC_HIT);
					pokemon_2->dir_hit = c1->callback->direction;
					pokemon_2->prev_position = pokemon_2->position;
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
	if (canmove % 3 != 0)
	{
		Move(dt);
	}
	else
	{
		walking = true;
	}
	if (canmove > 500)
	{
		canmove = 0;
	}
	canmove++;


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
			collision_attack = nullptr;
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
			collision_attack = App->collision->AddCollider({ position.x - 11, position.y - 37, 22, 8 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(10);
		}
		else if (direction == RIGHT)
		{
			collision_attack = App->collision->AddCollider({ position.x + 17, position.y - 26, 8, 22 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(10);
		}
		else if (direction == DOWN)
		{
			collision_attack = App->collision->AddCollider({ position.x - 10, position.y + 2, 22, 8 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(10);
		}
		else if (direction == LEFT)
		{
			collision_attack = App->collision->AddCollider({ position.x - 25, position.y - 26, 8, 22 }, COLLIDER_POKEMON_ATTACK, this);
			App->audio->PlayFx(10);
		}
	}
	return true;
}

void Salamance::Special_Attack()
{
	if (attacker)
	{
		if (wait_attack.ReadSec() > 3.5)
		{
			particle_fire->active = false;
		}
		if (current_animation->Finished() && wait_attack.ReadSec() > 4)
		{
			App->collision->EraseCollider(sp_attack);
			sp_attack = nullptr;
			attacker = false;
			current_animation->Reset();
			current_animation = nullptr;
			state = PC_IDLE;
			anim_state = PC_IDLE;
			getdamage = false;
			wait_attack.Start();
			App->particlemanager->DeleteFire_p(particle_fire);
			particle_fire = nullptr;
		}
	}
	else
	{
		attacker = true;
		wait_attack.Start();
		App->audio->PlayFx(32);
		if (direction == UP)
		{
			sp_attack = App->collision->AddCollider({ position.x - 11, position.y - 70, 16, 50 }, COLLIDER_POKEMON_SPECIAL_ATTACK, this);
			App->audio->PlayFx(10);
			//Particles
			App->particlemanager->CreateFire_Particle(nullptr, nullptr, iPoint(position.x - offset_x, position.y - offset_y), { 0,12,12,0 }, { 2,5 }, { 15, 10 }, { 0, 250 }, P_UP, 100, 2);
			particle_fire = App->particlemanager->Group_Fire.back();
		}
		else if (direction == RIGHT)
		{
			sp_attack = App->collision->AddCollider({ position.x + 10, position.y - 26, 50, 16 }, COLLIDER_POKEMON_SPECIAL_ATTACK, this);
			App->audio->PlayFx(10);
			//Particles
			App->particlemanager->CreateFire_Particle(nullptr, nullptr, iPoint(position.x - offset_x, position.y - offset_y), { 0,12,12,0 }, { 2,5 }, { 15, 10 }, { 250, 0 }, P_RIGHT, 100, 2);
			particle_fire = App->particlemanager->Group_Fire.back();
		}
		else if (direction == DOWN)
		{
			sp_attack = App->collision->AddCollider({ position.x - 10, position.y - 2, 16, 50 }, COLLIDER_POKEMON_SPECIAL_ATTACK, this);
			App->audio->PlayFx(10);
			//Particles
			App->particlemanager->CreateFire_Particle(nullptr, nullptr, iPoint(position.x - offset_x, position.y - offset_y), { 0,12,12,0 }, { 2,5 }, { 15, 10 }, { 0, 250 }, P_DOWN, 100, 2);
			particle_fire = App->particlemanager->Group_Fire.back();
		}
		else if (direction == LEFT)
		{
			sp_attack = App->collision->AddCollider({ position.x - 60, position.y - 26, 50, 16 }, COLLIDER_POKEMON_SPECIAL_ATTACK, this);
			App->audio->PlayFx(10);
			//Particles
			App->particlemanager->CreateFire_Particle(nullptr, nullptr, iPoint(position.x - offset_x, position.y - offset_y), { 0,12,12,0 }, { 2,5 }, { 15, 10 }, { 250, 0 }, P_LEFT, 100, 2);
			particle_fire = App->particlemanager->Group_Fire.back();
		}
		particle_fire->active = true;
	}
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
	if (canmove % 3 != 0)
	{
		Move(dt);
	}
	else
	{
		walking = true;
	}
	if (canmove > 500)
	{
		canmove = 0;
	}
	canmove++;


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

bool Salamance::Movebyhit(int speed)
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
	return true;
}

int Salamance::CheckPlayerPos()
{
	return App->combatmanager->pokemon_active_link->position.DistanceTo(position);
}