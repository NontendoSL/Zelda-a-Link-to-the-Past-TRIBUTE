#include "Ganon.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1AnimationManager.h"

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
	state = G_WALKING;
	anim_state = G_SHIELD;
	phase = INITIAL;

	//Set stats
	speed = 10;
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
		return true;
	}
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
			Walk(dt);
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
		phase = INVINCIBLE;
		state = G_SHIELD;
		spawn_timer.Start();
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
	}
	return true;
}

bool Ganon::RageUpdate(float dt)
{
	//Until Ganon is alive.
	if (hp > 0)
	{
		switch (state)
		{
		case G_IDLE:
			Idle();
			break;

		case G_WALKING:
			Walk(dt);
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
}

void Ganon::MeleeAttack()
{
}

void Ganon::SpecialAttack()
{
	// ATTACK PATTERN: 2 flares, then 1 circle attack.
	if (flare % 2 == 0 /*&& Animation finished*/)
	{
		special_attack = G_SPECIAL_2;
	}

	switch (special_attack)
	{
	case G_SPECIAL_1:
		Flare();
		break;

	case G_SPECIAL_2:
		FireCircle();
		break;

	default:
		break;
	}
}

void Ganon::Flare()
{
	flare++;
}

void Ganon::FireCircle()
{
	circle++;
}

void Ganon::Hit()
{
}

void Ganon::Spawn()
{
	// TODO -> TRY what type of enemies to spawn: Pokemon will be a great idea
	if (minions_spawned % 5 == 0)
	{
		//Spawn Miniboss
	}
	else
	{
		//Spawn normal soldier
	}
	minions_spawned++;
}

bool Ganon::CleanUp()
{
	return true;
}

void Ganon::OnCollision(Collider *, Collider *)
{
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




