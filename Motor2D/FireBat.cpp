#include "FireBat.h"
#include "j1Collision.h"
#include "j1EntityElementsScene.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "Ganon.h"

FireBat::FireBat()
{
	name = "FireBat";
	type = CREATURE;
}

FireBat::~FireBat()
{
}

bool FireBat::Start()
{
	//Load initial position & direction
	pos_in_vect = 0;
	position = App->entity_elements->ganon->GetPosinVect(pos_in_vect);
	direction = DOWN;

	//Load Stats
	hp = 1;
	speed = 1;
	attack = 5;

	//Animation
	state = B_SLEEP;
	anim_state = B_SLEEP;
	animation = *App->anim_manager->GetAnimStruct(FIREBAT);

	//Set Collision
	offset_x = 2;
	offset_y = 2;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 4, 4 }, COLLIDER_FIREBAT, this);

	//Spawn Timer
	spawn_time.Start();

	return true;
}

bool FireBat::Update(float dt)
{
	switch (state)
	{
	case B_SLEEP:
		if (App->entity_elements->ganon->start_awake == true && spawn_time.ReadSec() >= 4)
		{
			state = B_AWAKENING;
			anim_state = B_AWAKENING;
			animation.anim[anim_state].ResetAnimations();

		}
		break;
	case B_AWAKENING:
	{
		if (animation.anim[anim_state].South_action.Finished())
		{
			start_chase = true;
			origin = position;
			dest = App->scene->player->position;
			fly_timer.Start();
			state = B_FLYING;
			anim_state = B_FLYING;
		}
		break;
	}
	case B_FLYING:
	{
		if (start_chase == true && fly_timer.ReadSec() <= 2)
		{
			Fly();
		}
		else
		{
			state = B_DYING;
			anim_state = B_DYING;
		}
		break;
	}
	case B_DYING:
	{
		if (animation.anim[anim_state].South_action.Finished())
		{
			Die();
		}
	}
	}


	if (start_chase == false)
	{
		//FIREBAT MOVEMENT --------
		pos_in_vect += speed;
		if (pos_in_vect >= 180)
		{
			pos_in_vect = 0;
		}
		position = App->entity_elements->ganon->GetPosinVect(pos_in_vect);
		//------------------------------
	}

	collision_feet->SetPos(position.x, position.y);

	return true;
}

void FireBat::Draw()
{
	anim_rect = animation.anim[anim_state].South_action.GetCurrentFrame();
	pivot = animation.anim[anim_state].South_action.GetCurrentOffset();

	App->render->Blit(animation.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);
}

void FireBat::Fly()
{
	time = fly_timer.ReadSec();
	time *= 1.5;
	// QUADRATIC
	//position.x = (1 - time) * (1 - time)*origin.x + 2 * time * (1 - time) * (origin.x + 5) + time * time * dest.x;
	//position.y = (1 - time) * (1 - time)*origin.y + 2 * time * (1 - time) * (origin.y + 5) + time * time * dest.y;

	// LINEAR
	position.x = origin.x + (dest.x - origin.x) * time;
	position.y = origin.y + (dest.y - origin.y) * time;

}

bool FireBat::Die()
{
	to_delete = true;
	if (App->entity_elements->ganon != nullptr)
	{
		App->entity_elements->ganon->firebats_dead++;
	}
	return true;
}
