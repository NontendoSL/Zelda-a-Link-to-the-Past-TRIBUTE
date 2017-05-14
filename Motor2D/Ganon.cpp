#include "Ganon.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1AnimationManager.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "ParticleManager.h"
#include "j1Weapon.h"

Ganon::Ganon() :NPC()
{
	name = "Ganon";
	type = CREATURE;
	srand(time(NULL));
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
	position.y = 110;
	direction = DOWN;

	//Animation States & initial Phase
	state = G_IDLE;
	anim_state = G_WALKING;
	phase = SLEEP;

	//Set stats
	speed = 40;
	hp = 100;

	//Set Collision
	offset_x = 18;
	offset_y = 5;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 36, 22 }, COLLIDER_GANON, this);

	//Idle State Test
	wait_time.Start();
	return true;
}

bool Ganon::Update(float dt)
{
	App->gui->GetEntity("hp boss")->Hitbox.w = (hp * 149) / 100;
	BROFILER_CATEGORY("DoUpdate_Soldier", Profiler::Color::Red);
	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME || App->scene->gamestate == INMENU)
	{
		switch (phase)
		{
		//He sleeps until the fight starts.
		case SLEEP:
			InitCombat();
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
		direction = DOWN;
		spawn_timer.Start();
		LOG("SHIELD PHASE");
	}
	return true;
}

bool Ganon::InvincibleUpdate(float dt)
{

	if(hp<100)
		hp+=0.1f;
	// Until player hasn't killed a certain amount of enemies.
	if (minions_killed < 10)
	{
		if (spawn_timer.ReadSec() >= spawn_rate && minions_spawned < 10)
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
		hp = 100;
	}
	return true;
}

bool Ganon::RageUpdate(float dt)
{
	/*if (test) // TODO -> delete this
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
		firebat_rate = 1;
		new_fire_bat = 30;
	}*/

	if (collision_attack != nullptr && explosion_timer.ReadSec() >= 0.2)
	{
		collision_attack->to_delete = true;
	}

	//Until Ganon is alive.
	if (hp > 0)
	{
		switch (state)
		{
		case G_IDLE:
			IdleRage();
			break;

		case G_ATTACKING:
			SpecialAttack();
			break;

		case G_HIT:
			HitRage();
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
	Die();
	to_delete = true;
	return true;
}
bool Ganon::InitCombat()
{
	if (App->scene->player->position.y <= 195 && start_dialogue == false)
	{
		App->scene->player->dialog = App->gui->CreateDialogue("I'm Ganon, the Lord of the Shadow. You will never save the Two Worlds, Link. I'm gonna defeat you right now!");
		start_dialogue = true;
		LOG("START COMBAT");
		return true;
	}

	else if (start_dialogue == true && App->scene->player->dialog == nullptr)
	{
		start_dialogue = false;
		phase = INITIAL;
		state = G_IDLE;
		anim_state = G_WALKING;
		App->gui->GetEntity("boss bar")->visible = true;
		App->gui->GetEntity("hp boss")->visible = true;

		//Set Entrance Door blocked
		App->map->EditCost(32, 42, App->map->data.tilesets[0]->firstgid + 1);
		App->map->EditCost(32, 43, App->map->data.tilesets[0]->firstgid + 1);
		App->map->EditCost(33, 42, App->map->data.tilesets[0]->firstgid + 1);
		App->map->EditCost(33, 43, App->map->data.tilesets[0]->firstgid + 1);
		
		return true;
	}

	return false;
}
//--------------------------------------

void Ganon::IdleRage()
{
	if (wait_time.ReadSec() >= 0.7)
	{
		state = G_ATTACKING;
		anim_state = G_SPECIAL_2;
		special_attack = G_SPECIAL_2;
	}
}

void Ganon::Idle()
{
	if (wait_time.ReadSec() >= 0.7)
	{
		state = G_WALKING;
		anim_state = G_SPECIAL_1;
		StartJump();
	}
}

void Ganon::Walk(float dt)
{
	// CHECK JUMP MOVEMENT -----------------------
	if (position.DistanceTo(jump_dest) >= 5)
	{
		DoJump();
	}
	else
	{
		jump_finished = true;
	}
	// ---------------------------------------------

	// JUMP FINISHED ----------------------
	if (jump_finished == true)
	{
		ResetJump();
		Reorientate();
		state = G_ATTACKING;
		anim_state = G_MELEE;
		StartAttack = true;
		App->scene->swap_player = true;
	}
	// ---------------------------------------
}

void Ganon::StartMovement()
{
	Reorientate();
	mov_origin = position;
	mov_dest = App->scene->player->position;
	mov_timer.Start();
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
			collision_attack = App->collision->AddCollider({ position.x - offset_x - 6, position.y - offset_y - 10, 48, 48 }, COLLIDER_GANON_ATTACK, this);
		}
		else if (direction == UP)
		{
			collision_attack = App->collision->AddCollider({ position.x - offset_x - 6, position.y - offset_y - 30, 48, 53}, COLLIDER_GANON_ATTACK, this);
		}
	}

	else
	{
		if (current_animation->Finished())
		{
			collision_attack->to_delete = true;
			current_animation = nullptr;
			state = G_IDLE;
			anim_state = G_WALKING;
			wait_time.Start();
		}
	}
}

void Ganon::SpecialAttack()
{
	// ATTACK PATTERN: 1 Fire Bats, then 1 Jump and Circle attack.
	switch (special_attack)
	{
	case G_SPECIAL_1:
		FireJump();
		break;
	case G_SPECIAL_2:
		FireBats();
		break;
	default:
		break;
	}
}

void Ganon::FireBats()
{
	// Firebats Creation --------------------------------
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
	else
	{
		start_augment = true;
		attack_timer.Start();
	}
	// -----------------------------------------------

	// Firebats Awakening --------------------------------------
	if (start_augment == true)
	{
		if (augment_radius % 2 == 0)
		{
			if (ChangeRadius_degrade(50, true))
			{
				start_awake = true;
				start_augment = false;
			}
		}

		if (augment_radius > 500)
		{
			augment_radius = 0;
		}
		augment_radius++;
	}
	// -----------------------------------------------------

	// All FireBats are dead ---------
	if (firebats_dead >= max_spawns)
	{
		ResetFireBats();
		StartJump();
		state = G_ATTACKING;
		anim_state = G_SPECIAL_1;
		special_attack = G_SPECIAL_1;
	}
	//--------------------------------
}

void Ganon::ResetFireBats()
{
	radius = 40;
	firebats_dead = 0;
	num_firebats = 0;
	time_to_create = 0;
	start_awake = false;
	start_augment = false;
}

void Ganon::StartJump()
{
	App->scene->swap_ganon = true;
	Reorientate();
	jump_origin = position;
	jump_dest = App->scene->player->position;
	jump_timer.Start();
	num_jumps++;
}

void Ganon::FireJump()
{
	// CHECK JUMP MOVEMENT -----------------------
	if (position.DistanceTo(jump_dest) >= 5)
	{
		DoJump();
	}
	else 
	{
		jump_finished = true;
	}
	// ---------------------------------------------

	// JUMP FINISHED ----------------------
	if(num_jumps < max_jumps && jump_finished == true)
	{
		jump_finished = false;
		StartJump();
	}
	// ---------------------------------------

	// THIRD JUMP ----------
	else if (num_jumps == max_jumps && jump_finished == true)
	{
		//Set Attack Collider ---------------
		collision_attack = App->collision->AddCollider({ position.x - offset_x - 12, position.y - offset_y - 20, 60, 60 }, COLLIDER_GANON_ATTACK, this);
		//----------------------------

		App->particlemanager->CreateExplosion_Particle(nullptr, nullptr, position, SDL_Rect{ 0, 4, 2, 0 }, CIRCLE, iPoint(10, 10), iPoint(10, 2), fPoint(60, 60), P_RANDOM, 22);

		float factor = (float)M_PI / 180.0f * MULTI_P;
		for (uint i = 0; i < NUM_POINTS_CIRCLE; ++i)
		{
			points[i].x = (int)(position.x + radius * cos(i * factor));
			points[i].y = (int)(position.y + radius * sin(i * factor));
		}
		Reorientate();
		ResetJump();
		explosion_timer.Start();
		wait_time.Start();
		state = G_IDLE;
		anim_state = G_WALKING;
		App->scene->swap_player = true;
	}
	// -----------------------------
}

void Ganon::DoJump()
{
	jump_time = jump_timer.ReadSec();
	//jump_time *= 1.5;
	// QUADRATIC
	//position.x = (1 - jump_time) * (1 - jump_time)*jump_origin.x + 2 * jump_time * (1 - jump_time) * (jump_origin.x + 5) + jump_time * jump_time * jump_dest.x;
	position.y = (1 - jump_time) * (1 - jump_time)*jump_origin.y + 2 * jump_time * (1 - jump_time) * (jump_origin.y - 50) + jump_time * jump_time * jump_dest.y;

	// LINEAR
	position.x = jump_origin.x + (jump_dest.x - jump_origin.x) * jump_time;
	//position.y = jump_origin.y + (jump_dest.y - jump_origin.y) * jump_time;
}

void Ganon::ResetJump()
{
	num_jumps = 0;
	jump_finished = false;
}

void Ganon::Hit()
{
	if (HitTime.ReadSec() >= 0.7)
	{
		state = G_IDLE;
		anim_state = G_WALKING;
	}
}

void Ganon::HitRage()
{
	if (HitTime.ReadSec() >= 0.7)
	{
		state = G_IDLE;
		anim_state = G_WALKING;
	}
}

void Ganon::Spawn()
{
	if (minions_spawned % 2 == 0 && minions_spawned > 0)
	{
		App->entity_elements->CreateRMinion({ 200 + (rand() % 100), 200 + (rand() % 100) });
	}

	else
	{
		App->entity_elements->CreateGMinion({ 200 + (rand() % 100), 200 + (rand() % 100) });
	}

	minions_spawned++;
}

bool Ganon::Die()
{
	App->gui->GetEntity("boss bar")->visible = false;
	App->gui->GetEntity("hp boss")->visible = false;
	App->gui->GetEntity("YOU WIN")->visible = true;
	App->scene->win_timer = SDL_GetTicks();
	return true;
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
			if (state != G_HIT)
			{
				if (phase == INITIAL && state == G_IDLE)
				{
					App->audio->PlayFx(12);
					HitTime.Start();
					hp -= 10;
					state = G_HIT;
					anim_state = G_HIT;
				}
				else if (phase == RAGE && state == G_IDLE)
				{
					App->audio->PlayFx(12);
					HitTime.Start();
					hp -= 10;
					state = G_HIT;
					anim_state = G_HIT;
				}
			}
		}

		// ARROW COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_ARROW && c2->arrow_callback != nullptr)
		{
			if (c2->arrow_callback->step == AIR && state != G_HIT) //&& phase != INVINCIBLE)
			{
				if (phase == INITIAL && state == G_IDLE)
				{
					App->audio->PlayFx(12);
					knockback_time.Start();
					hp -= 10;
					state = G_HIT;
					anim_state = G_HIT;
					dir_hit = c2->arrow_callback->direction;
					prev_position = position;
					c2->arrow_callback->step = IMPACT;
				}
				else if (phase == RAGE && state == G_IDLE)
				{
					App->audio->PlayFx(12);
					knockback_time.Start();
					hp -= 10;
					state = G_HIT;
					anim_state = G_HIT;
					dir_hit = c2->arrow_callback->direction;
					prev_position = position;
					c2->arrow_callback->step = IMPACT; 
				}		
			}
		}

		// BOMB COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_BOMB)
		{
			if (state != G_HIT)
			{
				if (phase == INITIAL && state == G_IDLE)
				{
					App->audio->PlayFx(12);
					HitTime.Start();
					hp -= 10;
					state = G_HIT;
					anim_state = G_HIT;
				}
				else if (phase == RAGE && state == G_IDLE)
				{
					App->audio->PlayFx(12);
					HitTime.Start();
					hp -= 10;
					state = G_HIT;
					anim_state = G_HIT;
				}
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
	if (index >= 0 && index < points.size())
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

void Ganon::IncreaseDeadMinions()
{
	minions_killed++;
}

void Ganon::SetDestination()
{
	jump_dest = App->scene->player->position;
	switch (App->scene->player->direction)
	{
	case UP:
		jump_dest.y -= 10;
		break;
	case DOWN:
		jump_dest.y += 10;
		break;
	case LEFT:
		jump_dest.x -= 10;
		break;
	case RIGHT:
		jump_dest.x += 10;
		break;
	default:
		break;
	}
}




