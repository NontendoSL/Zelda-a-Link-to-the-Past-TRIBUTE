#include "FireBat.h"
#include "j1Collision.h"
#include "j1EntityElementsScene.h"
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
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 4, 4 }, COLLIDER_GMINION, this);

	//Spawn Timer
	spawn_time.Start();

	return true;
}

bool FireBat::Update(float dt)
{
	pos_in_vect += speed * 2;
	if (pos_in_vect >= 180)
	{
		pos_in_vect = 0;
	}
	position = App->entity_elements->ganon->GetPosinVect(pos_in_vect);
	collision_feet->SetPos(position.x, position.y);
	return true;
}

void FireBat::Draw()
{
	anim_rect = animation.anim[anim_state].South_action.GetCurrentFrame();
	pivot = animation.anim[anim_state].South_action.GetCurrentOffset();

	App->render->Blit(animation.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);
}
