#include "Ganon.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1AnimationManager.h"
#include "j1Audio.h"
#include "j1Player.h"

Ganon::Ganon() :NPC()
{
	name = "Ganon";
	type = CREATURE;
}

Ganon::~Ganon()
{}

bool Ganon::Awake(pugi::xml_node &conf, uint id)
{
	return true;
}

bool Ganon::Start()
{
	//Load initial position & direction
	position.x = 264;
	position.y = 200;
	direction = DOWN;

	//Animation States & initial Phase
	state = G_ATTACKING;
	anim_state = G_SPECIAL_2;
	phase = RAGE;

	//Set stats
	speed = 40;
	hp = 100;

	//Set Collision
	offset_x = 18;
	offset_y = 5;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 36, 22 }, COLLIDER_GANON, this);

	return true;
}

bool Ganon::Update(float dt)
{
	BROFILER_CATEGORY("DoUpdate_Soldier", Profiler::Color::Red);
	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME)
	{
		switch (phase)
		{
		//He sleeps until the fight starts.
		case SLEEP:
			break;

		//First phase: normal melee attacks, low speed.
		case INITIAL:
			InitialUpdate(dt);
			break;

		//Second phase: invincible, spawning minions.
		case INVINCIBLE:
			InvincibleUpdate(dt);
			break;

		//Final phase: medium-high speed, fire attacks.
		case RAGE:
			RageUpdate(dt);
			break;

		//Ganon defeated, GG WP.
		case DEATH:
			DeathUpdate(dt);
			break;

		default:
			break;
		}

		if (collision_feet != nullptr)
		{
			collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
		}
	}
	return true;
}

void Ganon::Draw()
{
	App->anim_manager->Drawing_Manager(anim_state, direction, position, GANON);
}

//PHASE UPDATES MACHINE ------------------
bool Ganon::InitialUpdate(float dt)
{
	// DO "INITIAL" ACTIONS UNTIL HP IS ABOVE CERTAIN AMOUNT
	if (hp >= 50)
	{
		switch (state)
		{
		case G_IDLE:
			Idle();
			break;

		case G_HIT:
			Hit();
			break;

		case G_WALKING:
			Orientate();
			Walk(dt);
			Reorientate();
			break;

		case G_ATTACKING:
			MeleeAttack();
			break;

		default:
			break;
		}
	}

	// START NEXT PHASE
	else
	{
		position.x = 264;
		position.y = 110;
		collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
		phase = INVINCIBLE;
		state = G_SHIELD;
		anim_state = G_SHIELD;
		spawn_timer.Start();
		LOG("SHIELD PHASE");
	}
	return true;
}

bool Ganon::InvincibleUpdate(float dt)
{
	// Until player hasn't killed a certain amount of enemies.
	if (minions_killed < 20)
	{
		if (spawn_timer.ReadSec() >= spawn_rate)
		{
			Spawn();
			spawn_timer.Start();
		}
	}

	// START NEXT PHASE
	else
	{
		phase = RAGE;
		float factor = (float)M_PI / 180.0f * MULTI_P;
		for (uint i = 0; i < NUM_POINTS_CIRCLE; ++i)
		{
			iPoint temp;
			temp.x = (int)(position.x + radius * cos(i * factor));
			temp.y = (int)(position.y + radius * sin(i * factor));
			points.push_back(temp);
		}

		state = G_ATTACKING;
		special_attack = G_SPECIAL_2;
		anim_state = G_SPECIAL_2;
	}
	return true;
}

bool Ganon::RageUpdate(float dt)
{
	if (test)
	{
		float factor = (float)M_PI / 180.0f * MULTI_P;
		for (uint i = 0; i < NUM_POINTS_CIRCLE; ++i)
		{
			iPoint temp;
			temp.x = (int)(position.x + radius * cos(i * factor));
			temp.y = (int)(position.y + radius * sin(i * factor));
			points.push_back(temp);
		}
		state = G_ATTACKING;
		special_attack = G_SPECIAL_2;
		anim_state = G_SPECIAL_2;
		test = false;
		firebat_rate = 2;
		new_fire_bat = 30;
	}
	//Until Ganon is alive.
	if (hp > 0)
	{
		switch (state)
		{
		case G_IDLE:
			Idle();
			break;

		case G_WALKING:
			Orientate();
			Walk(dt);
			Reorientate(); //Ganon only has 2 directions: UP & DOWN.
			break;

		case G_ATTACKING:
			SpecialAttack();
			break;

		case G_HIT:
			Hit();
			break;
			
		default:
			break;
		}
	}

	// GANON IS DEAD
	else
	{
		phase = DEATH;
		state = G_DYING;
	}
	return true;
}

bool Ganon::DeathUpdate(float dt)
{
	/*if (player->celdaball->empty == true)
	{
		//PLAY POKEBALL CINEMATIC
	}

	else
	{
		//PLAY NORMAL DEATH CINEMATIC
	}*/
	return true;
}
//--------------------------------------

void Ganon::Idle()
{
}

void Ganon::Walk(float dt)
{
	if (canmove % 2 == 0)
	{
		Move(dt);
	}

	if (canmove > 500)
	{
		canmove = 0;
	}
	canmove++;
}

bool Ganon::Move(float dt)
{
	if (direction == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - ceil(speed*dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= ceil(speed*dt);
		}
	}

	else if (direction == RIGHT)
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + ceil(speed*dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += ceil(speed*dt);
		}
	}

	else if (direction == UP)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - ceil(speed*dt), collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			position.y -= ceil(speed*dt);
		}
	}

	else if (direction == DOWN)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + ceil(speed*dt), collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += ceil(speed*dt);
		}
	}

	walking = true;
	return true;
}

void Ganon::Reorientate()
{
	if (App->scene->player->position.y > position.y)
	{
		direction = DOWN;
	}
	else
	{
		direction = UP;
	}
}

void Ganon::MeleeAttack()
{
	if (StartAttack == true)
	{
		//Set Attack Animation
		StartAttack = false;
		current_animation = App->anim_manager->GetAnimation(anim_state, direction, GANON);
		current_animation->Reset();

		//Set Attack Collider
		if (direction == DOWN)
		{
			collision_attack = App->collision->AddCollider({ position.x - offset_x - 6, position.y - offset_y - 10, 48, 48 }, COLLIDER_GANON_FORK, this);
		}
		else if (direction == UP)
		{
			collision_attack = App->collision->AddCollider({ position.x - offset_x - 6, position.y - offset_y - 15, 48, 48 }, COLLIDER_GANON_FORK, this);
		}
	}

	else
	{
		if (current_animation->Finished())
		{
			collision_attack->to_delete = true;
			current_animation = nullptr;
			state = G_WALKING;
			anim_state = G_WALKING;
		}
	}
}

void Ganon::SpecialAttack()
{
	// ATTACK PATTERN: 1 Fire Bats, then 1 Jump and Circle attack.
	switch (special_attack)
	{
	case G_SPECIAL_2:
		FireBats();
		break;

	case G_SPECIAL_1:
		FireCircle();
		break;

	default:
		break;
	}
}

void Ganon::FireBats()
{
	if (num_firebats < max_spawns)
	{
		time_to_create += firebat_rate;
		if (time_to_create >= new_fire_bat && num_firebats < max_spawns)
		{
			App->entity_elements->CreateFireBat();
			num_firebats++;
			time_to_create = 0;
		}
	}
	else if (ChangeRadius_degrade(70, true))
	{

	}
	else
	{

	}
}

void Ganon::FireCircle()
{
	anim_state = G_SPECIAL_2;
	special_attack = G_SPECIAL_1;
}

void Ganon::Hit()
{
	if (HitTime.ReadSec() >= 0.3)
	{
		state = G_ATTACKING;
		anim_state = G_MELEE;
		StartAttack = true;
	}
	if (collision_attack != nullptr)
	{
		collision_attack->to_delete = true;
	}
}

void Ganon::Spawn()
{
	// TODO -> TRY what type of enemies to spawn: Pokemon will be a great idea
	if (minions_spawned % 5 == 0 && minions_spawned > 0)
	{
		App->entity_elements->CreateRMinion({ 300, 200 });
	}
	else
	{
		App->entity_elements->CreateGMinion({ 200, 200 });
	}
	minions_spawned++;
}

bool Ganon::CleanUp()
{
	return true;
}

void Ganon::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		// SWORD COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_SWORD)
		{
			if (state != G_HIT && state != G_ATTACKING)
			{
				App->audio->PlayFx(12);
				HitTime.Start();
				hp -= 10;
				state = G_HIT;
				anim_state = G_HIT;
				LOG("HP:%i", hp);
			}
		}

		// PLAYER COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_PLAYER)
		{
			if (state != G_HIT && state != G_ATTACKING)
			{
				state = G_ATTACKING;
				anim_state = G_MELEE;
				StartAttack = true;
			}
		}
	}
}

GanonState Ganon::GetState()
{
	return state;
}

void Ganon::SetState(GanonState s_state)
{
	state = s_state;
}

void Ganon::SetAnimState(GanonState a_state)
{
	anim_state = a_state;
}

iPoint Ganon::GetPosinVect(int index)
{
	if (index > 0 && index < points.size())
	{
		return points[index];
	}
	else
	{
		return{ 0,0 };
	}
}

bool Ganon::ChangeRadius_degrade(int radius_to_stop, bool increment)
{
	if (increment && radius != radius_to_stop)
	{
		radius += 1;
		float factor = (float)M_PI / 180.0f * MULTI_P;
		for (uint i = 0; i < NUM_POINTS_CIRCLE; ++i)
		{
			points[i].x = (int)(position.x + radius * cos(i * factor));
			points[i].y = (int)(position.y + radius * sin(i * factor));
		}
		if (radius == radius_to_stop)
		{
			return true;
		}
	}
	else if (radius != radius_to_stop)
	{
		radius -= 1;
		float factor = (float)M_PI / 180.0f * MULTI_P;
		for (uint i = 0; i < NUM_POINTS_CIRCLE; ++i)
		{
			points[i].x = (int)(position.x + radius * cos(i * factor));
			points[i].y = (int)(position.y + radius * sin(i * factor));
		}
		if (radius == radius_to_stop)
		{
			return true;
		}
	}
	return false;
}




