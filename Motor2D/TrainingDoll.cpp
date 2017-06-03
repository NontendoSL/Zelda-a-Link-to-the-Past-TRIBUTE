#include "TrainingDoll.h"
#include "j1Scene.h"
#include "CombatManager.h"

TrainingDoll::TrainingDoll()
{
	type = CREATURE;
}

TrainingDoll::~TrainingDoll()
{
}

bool TrainingDoll::Awake(pugi::xml_node& conf)
{
	direction = DOWN;
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

bool TrainingDoll::Start()
{
	state = PC_IDLE;
	anim_state = PC_IDLE;
	scale = App->win->GetScale();
	offset_x = 7;
	offset_y = 17;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 15, 15 }, COLLIDER_TRANINGDOLL, this);
	return true;
}

bool TrainingDoll::Update(float dt)
{
	BROFILER_CATEGORY("Update_Collision", Profiler::Color::LightSalmon);

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
		case PC_HIT:
		{
			Hit();
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
	return true;
}

void TrainingDoll::Draw()
{
	BROFILER_CATEGORY("Draw_TrainingDoll", Profiler::Color::Gainsboro);
	App->anim_manager->Drawing_Manager(anim_state, direction, position, TRAININGDOLL);
}

bool TrainingDoll::CleanUp()
{
	return true;
}

bool TrainingDoll::Idle()
{
	return true;
}

void TrainingDoll::Hit()
{
	if (current_animation->Finished())
	{
		current_animation->Reset();
		current_animation = nullptr;
		state = PC_IDLE;
		anim_state = PC_IDLE;
	}
}

void TrainingDoll::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		PokemonCombat* pokemon = (PokemonCombat*)c2->callback;
		if (c1 == collision_feet && c2->type == COLLIDER_POKEMON_ATTACK && state != PC_HIT)
		{
			hp -= pokemon->attack;
			anim_state = PC_WALKING;
			state = PC_HIT;
			current_animation = App->anim_manager->GetAnimation(anim_state, direction, TRAININGDOLL);
			current_animation->Reset();
		}
		if (c1 == collision_feet && c2->type == COLLIDER_POKEMON_SPECIAL_ATTACK && state != PC_HIT)
		{
			hp -= pokemon->sp_damage;
			anim_state = PC_WALKING;
			state = PC_HIT;
			current_animation = App->anim_manager->GetAnimation(anim_state, direction, TRAININGDOLL);
			current_animation->Reset();
		}
		if (c1 == collision_feet && c2->type == COLLIDER_POKECOMBAT)
		{
			pokemon->SetState(PC_COLLISION);
			pokemon->SetAnimState(PC_IDLE);
			if (pokemon->direction == UP)
			{
				pokemon->dir_hit = DOWN;
			}
			else if (pokemon->direction == DOWN)
			{
				pokemon->dir_hit = UP;
			}
			else if (pokemon->direction == RIGHT)
			{
				pokemon->dir_hit = LEFT;
			}
			else
			{
				pokemon->dir_hit = RIGHT;
			}
			pokemon->knockback_time.Start();
		}
	}
}
