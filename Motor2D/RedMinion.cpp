#include "RedMinion.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "Ganon.h"
#include "j1EntityElementsScene.h"
#include "j1Weapon.h"
RedMinion::RedMinion()
{
	name = "RedMinion";
	type = CREATURE;
}


RedMinion::~RedMinion()
{
}

bool RedMinion::Start(iPoint pos)
{
	//Load initial position & direction
	position.x = pos.x;
	position.y = pos.y;
	direction = DOWN;

	//Load Stats
	hp = 1;
	speed = 70;
	attack = 10;

	//Animation
	state = RM_SPAWNING;
	anim_state = RM_SPAWNING;
	animation = *App->anim_manager->GetAnimStruct(REDMINION);
	//explosion_anim = *App->anim_manager->GetAnimStruct(BOMB);
	animation.anim[anim_state].ResetAnimations();
	//explosion_anim.anim[W_DYING].ResetAnimations();
	death_graphics = App->tex->Load("textures/AnimationsAndEffects.png");
	explosion = App->tex->Load("textures/Items.png");

	//Set Collision
	offset_x = 8;
	offset_y = 4;

	//Spawn Timer
	spawn_time.Start();

	//Orient Timer
	change_dir.Start();

	return true;
}

bool RedMinion::Update(float dt)
{
	BROFILER_CATEGORY("Update_Collision", Profiler::Color::CornflowerBlue);

	if (App->scene->gamestate == INGAME)
	{
		if (start_explosion == true)
		{
			App->audio->PlayFx(7);
			state = RM_EXPLODING;
			anim_state = RM_EXPLODING;
			animation.anim[RM_EXPLODING].ResetAnimations();
			direction = DOWN;

			if (explosion != nullptr)
			{
				animation.graphics = explosion;
			}
			start_explosion = false;
		}

		switch (state)
		{
		case RM_SPAWNING:
		{
			Spawning();
			break;
		}
		case RM_IDLE:
		{
			Idle();
			break;
		}
		case RM_WALKING:
		{
			Orientate();
			Walking(dt);
			break;
		}
		case RM_HIT:
		{
			Movebyhit(dt);
			break;
		}
		case RM_DYING:
		{
			Die();
			break;
		}
		case RM_EXPLODING:
		{
			Explode();
			break;
		}
		default:
		{
			break;
		}
		}
	}

	if (collision_feet != nullptr)
	{
		collision_feet->SetPos(position.x - offset_x, position.y - offset_y);
	}
	return true;
}

void RedMinion::Draw()
{
	BROFILER_CATEGORY("Draw_SOLDIER", Profiler::Color::Crimson);
	if (direction == UP)
	{
		anim_rect = animation.anim[anim_state].North_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].North_action.GetCurrentOffset();
	}
	else if (direction == DOWN)
	{
		anim_rect = animation.anim[anim_state].South_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].South_action.GetCurrentOffset();
	}
	else if (direction == LEFT)
	{
		anim_rect = animation.anim[anim_state].West_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].West_action.GetCurrentOffset();
	}
	else if (direction == RIGHT)
	{
		anim_rect = animation.anim[anim_state].East_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].East_action.GetCurrentOffset();
	}

	App->render->Blit(animation.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);


	//else
	//{
	//	anim_rect = explosion_anim.anim[W_DYING].South_action.GetCurrentFrame();
	//	pivot = explosion_anim.anim[W_DYING].South_action.GetCurrentOffset();
	//	App->render->Blit(explosion_anim.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);
	//}
}

void RedMinion::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		//SWORD COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_SWORD && c1->callback != nullptr)
		{
			if (state != RM_HIT && state != RM_DYING && state != RM_EXPLODING)
			{
				App->audio->PlayFx(11);
				knockback_time.Start();
				hp--;
				state = RM_HIT;
				anim_state = RM_WALKING;
				dir_hit = c2->callback->direction;
				prev_position = position;
			}
		}

		// ARROW COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_ARROW && c2->arrow_callback != nullptr)
		{
			if (c2->arrow_callback->step == AIR && state != RM_HIT && state != RM_EXPLODING)
			{
				App->audio->PlayFx(12);
				knockback_time.Start();
				hp--;
				state = RM_HIT;
				anim_state = RM_WALKING;
				dir_hit = c2->arrow_callback->direction;
				prev_position = position;
				c2->arrow_callback->step = ENEMY_IMPACT; // TODO MED -> set step to impact: this will reproduce the impact animation and, when finished, set step to DIE.
			}
		}

		////BOMB COLLISION
		//if (c1 == collision_feet && c2->type == COLLIDER_BOMB && c1->callback != nullptr)
		//{
		//	if (state != RM_HIT)
		//	{
		//		App->audio->PlayFx(12);
		//		knockback_time.Start();
		//		hp--;
		//		state = RM_HIT;
		//		anim_state = RM_WALKING;
		//		SetKnockbackDir();
		//		prev_position = position;
		//	}
		//}
	}
}

bool RedMinion::Spawning()
{
	if (spawn_time.ReadSec() >= 1)
	{
		collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 17, 17 }, COLLIDER_RMINION, this);
		state = RM_WALKING;
		anim_state = RM_WALKING;
	}
	return true;
}

bool RedMinion::Idle()
{
	return true;
}

bool RedMinion::Walking(float dt)
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

	return true;
}

bool RedMinion::Move(float dt)
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

	dis_moved++;
	walking = true;

	return true;
}

bool RedMinion::Movebyhit(float dt)
{
	//CHECK FOR DEATH
	if (hp <= 0)
	{
		App->audio->PlayFx(11);
		state = RM_DYING;
		anim_state = RM_DYING;
		animation.anim[RM_DYING].ResetAnimations();
		direction = DOWN;

		if (collision_feet != nullptr)
		{
			//Erase Collider
			collision_feet->to_delete = true;
			collision_feet = nullptr;
		}

		if (death_graphics != nullptr)
		{
			animation.graphics = death_graphics;
		}
	}

	else
	{
		//CHECK FOR RESET WALKING
		if (knockback_time.ReadSec() >= 0.2)
		{
			state = RM_WALKING;
			anim_state = RM_WALKING;
			return true;
		}

		//CHECK HIT DIRECTION -------
		if (dir_hit == UP)
		{
			if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - ceil(100 * dt), collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
			{
				position.y -= ceil(100 * dt);
			}
		}
		else if (dir_hit == DOWN)
		{
			if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + ceil(100 * dt), collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
			{
				position.y += ceil(100 * dt);
			}
		}
		else if (dir_hit == LEFT)
		{
			if (App->map->MovementCost(collision_feet->rect.x - ceil(100 * dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
			{
				position.x -= ceil(100 * dt);
			}
		}
		else if (dir_hit == RIGHT)
		{
			if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + ceil(100 * dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
			{
				position.x += ceil(100 * dt);
			}
		}
		//------------------------------
	}
	return true;
}

bool RedMinion::Die()
{
	if (animation.anim[anim_state].South_action.Finished() == true)
	{
		App->audio->PlayFx(11);

		if (item_id != -1)
		{
			App->entity_elements->CreateItem(DropItem(), position);
		}

		if (App->entity_elements->ganon != nullptr)
		{
			App->entity_elements->ganon->IncreaseDeadMinions();
		}

		to_delete = true;
	}

	return true;
}

bool RedMinion::Explode()
{
	if (animation.anim[anim_state].South_action.Finished() == true)
	{
		if (item_id != -1)
		{
			App->entity_elements->CreateItem(DropItem(), position);
		}

		if (App->entity_elements->ganon != nullptr)
		{
			App->entity_elements->ganon->IncreaseDeadMinions();
		}

		to_delete = true;
	}

	return true;
}

void RedMinion::SetKnockbackDir()
{
	if (direction == UP)
	{
		dir_hit = DOWN;
	}
	else if (direction == DOWN)
	{
		dir_hit = UP;
	}
	else if (direction == LEFT)
	{
		dir_hit = RIGHT;
	}
	else
	{
		dir_hit = LEFT;
	}
}

int RedMinion::DropItem()
{
	item_id = 2;
	return item_id;
}

void RedMinion::SetState(RMinionState s_state)
{
	state = s_state;
}

void RedMinion::SetAnimState(RMinionState a_state)
{
	anim_state = a_state;
}

RMinionState RedMinion::GetState() const
{
	return state;
}



