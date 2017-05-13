#include "GreenMinion.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "Ganon.h"
#include "j1EntityElementsScene.h"
#include "j1Weapon.h"

GreenMinion::GreenMinion()
{
	name = "GreenMinion";
	type = CREATURE;
}


GreenMinion::~GreenMinion()
{
}

bool GreenMinion::Start(iPoint pos)
{
	//Load initial position & direction
	position.x = pos.x;
	position.y = pos.y;
	direction = DOWN;

	//Load Stats
	hp = 2;
	speed = 40;
	attack = 10;

	//Animation
	state = GM_SPAWNING;
	anim_state = GM_WALKING;
	animation = *App->anim_manager->GetAnimStruct(GREENMINION);
	animation.anim[anim_state].ResetAnimations();
	//Set Collision
	offset_x = 8;
	offset_y = 4;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 17, 17 }, COLLIDER_GMINION, this);

	//Spawn Timer
	spawn_time.Start();

	return true;
}

bool GreenMinion::Update(float dt)
{
	if (App->scene->gamestate == INGAME)
	{
		switch (state)
		{
		case GM_SPAWNING:
		{
			Spawning();
			break;
		}
		case GM_IDLE:
		{
			Idle();
			break;
		}
		case GM_WALKING:
		{
			Orientate();
			Walking(dt);
			break;
		}
		case GM_HIT:
		{
			Movebyhit(dt);
			break;
		}
		case GM_DYING:
		{
			Die();
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

void GreenMinion::Draw()
{
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
}

void GreenMinion::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		//SWORD COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_SWORD && c1->callback != nullptr)
		{
			if (state != GM_HIT)
			{
				App->audio->PlayFx(12);
				knockback_time.Start();
				hp--;
				state = GM_HIT;
				anim_state = GM_WALKING;
				dir_hit = c2->callback->direction;
				prev_position = position;
			}
		}

		//ARROW COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_ARROW && c2->arrow_callback != nullptr)
		{
			if (c2->arrow_callback->step == AIR && state != GM_HIT)
			{
				App->audio->PlayFx(12);
				knockback_time.Start();
				hp--;
				state = GM_HIT;
				anim_state = GM_WALKING;
				dir_hit = c2->arrow_callback->direction;
				prev_position = position;
				c2->arrow_callback->step = IMPACT; // TODO MED -> set step to impact: this will reproduce the impact animation and, when finished, set step to DIE.
			}
		}

		//BOMB COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_BOMB && c1->callback != nullptr)
		{
			if (state != GM_HIT)
			{
				App->audio->PlayFx(12);
				knockback_time.Start();
				hp--;
				state = GM_HIT;
				anim_state = GM_WALKING;
				SetKnockbackDir();
				prev_position = position;
			}
		}
	}
}

bool GreenMinion::Spawning()
{
	if (spawn_time.ReadSec() >= 0.7)
	{
		state = GM_WALKING;
		anim_state = GM_WALKING;
	}

	return true;
}

bool GreenMinion::Idle()
{
	return true;
}

bool GreenMinion::Walking(float dt)
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

bool GreenMinion::Move(float dt)
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

bool GreenMinion::Movebyhit(float dt)
{
	//CHECK FOR DEATH
	if (hp <= 0)
	{
		state = GM_DYING;
		anim_state = GM_IDLE;
		return true;
	}

	//CHECK FOR RESET WALKING
	if (knockback_time.ReadSec() >= 0.2)
	{
		state = GM_WALKING;
		anim_state = GM_WALKING;
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

	return true;
}

bool GreenMinion::Die()
{
	App->audio->PlayFx(11);
	if (item_id != -1)
	{
		App->entity_elements->CreateItem(item_id, position);
	}

	if (App->entity_elements->ganon != nullptr)
	{
		App->entity_elements->ganon->IncreaseDeadMinions();
	}

	to_delete = true;

	return true;
}

void GreenMinion::SetKnockbackDir()
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



