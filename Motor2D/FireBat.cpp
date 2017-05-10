#include "FireBat.h"
#include "j1Collision.h"

FireBat::FireBat()
{
	name = "FireBat";
	type = CREATURE;
}

FireBat::~FireBat()
{
}

bool FireBat::Start(iPoint* path_pos)
{
	//Load initial position & direction
	path = path_pos;
	direction = DOWN;

	//Load Stats
	hp = 1;
	speed = 60;
	attack = 5;

	//Animation
	state = B_SLEEP;
	anim_state = B_SLEEP;
	animation = *App->anim_manager->GetAnimStruct(FIREBAT);

	//Set Collision
	offset_x =2;
	offset_y = 2;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 4, 4 }, COLLIDER_GMINION, this);

	//Spawn Timer
	spawn_time.Start();

	return true;
}

bool FireBat::Update(float dt)
{

	return false;
}

void FireBat::Draw()
{
}
