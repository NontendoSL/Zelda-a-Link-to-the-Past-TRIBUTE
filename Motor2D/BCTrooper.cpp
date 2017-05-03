#include "BCTrooper.h"
#include "j1Scene.h"
#include "ParticleManager.h"
#include "j1Collision.h"
#include "j1Player.h"

BCTrooper::BCTrooper() : NPC()
{
	name = "BCTrooper";
	type = CREATURE;
}

BCTrooper::~BCTrooper()
{}

bool BCTrooper::Awake(pugi::xml_node &conf, uint id)
{

	return true;
}

bool BCTrooper::Start()
{
	texture = App->tex->Load("Particles/bctrooperl.png");
	position.x = 200;
	position.y = 250;
	float factor = (float)M_PI / 180.0f * MULTI_P;
	for (uint i = 0; i < NUM_POINTS_CIRCLE; ++i)
	{
		iPoint temp;
		temp.x = (int)(position.x + radius * cos(i * factor));
		temp.y = (int)(position.y + radius * sin(i * factor));
		points.push_back(temp);
	}
	bole.x = points.begin()._Ptr->x;
	bole.y = points.begin()._Ptr->y;
	prev_position = position;
	speed = 1;
	collision_feet = App->collision->AddCollider({ position.x, position.y, 16, 15 }, COLLIDER_BCTROOPER, this);
	collision_maze = App->collision->AddCollider({ bole.x, bole.y, 14, 14 }, COLLIDER_BCTROOPER_MAZE, this);
	App->particlemanager->CreateFollow_P(nullptr, &bole, SDL_Rect{ 0,10,2,0 }, iPoint(5,5), iPoint(18,8), 4, 30, true);
	return true;
}

bool BCTrooper::Update(float dt)
{
	BROFILER_CATEGORY("DoUpdate_BCTrooper", Profiler::Color::Red);
	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME)
	{
		switch (state)
		{
			//
		case BC_IDLE:
			Idle();
			break;

			//
		case BC_WALKING:
			Walk(dt);
			break;

			//
		case BC_HIT:
			Hit();
			break;

			//
		case BC_DYING:
			Death();
			break;

		default:
			break;
		}
	}

	if (position.x != prev_position.x || position.y != prev_position.y)
	{
		prev_position = position;
		float factor = (float)M_PI / 180.0f * MULTI_P;
		for (uint i = 0; i < NUM_POINTS_CIRCLE; ++i)
		{
			points[i].x = (int)(position.x + radius * cos(i * factor));
			points[i].y = (int)(position.y + radius * sin(i * factor));
		}
	}
	//
	pos_in_vect += speed;
	if (pos_in_vect >= NUM_POINTS_CIRCLE)
	{
		pos_in_vect = 1;
	}
	bole.x = points[pos_in_vect].x;
	bole.y = points[pos_in_vect].y;
	//Update colliders position
	collision_feet->SetPos(position.x, position.y);
	collision_maze->SetPos(bole.x, bole.y);
	return true;
}

void BCTrooper::Draw()
{
	if (App->collision->Getdebug())
	{
		SDL_Rect temp = { 20,12,2,2 };
		for (int i = 0; i < NUM_POINTS_CIRCLE; i++)
		{
			App->render->Blit(texture, points[i].x, points[i].y, &temp);
		}
	}


	SDL_Rect temp_3 = { 0,0,radius,6 };
	App->render->Blit(texture, position.x, position.y, &temp_3, 1, true, pos_in_vect * MULTI_P, 5, 5);
	

	SDL_Rect temp_2 = { 0,7,14,14 };
	App->render->Blit(texture, bole.x - 5, bole.y - 4, &temp_2);

}


bool BCTrooper::CleanUp()
{
	return true;
}

void BCTrooper::Idle()
{
	if (reset_time)
	{
		Change_State.Start();
		reset_time = false;
	}
	else
	{
		if (Change_State.ReadSec() > 1)
		{
			if (ChangeRadius(50, true))
			{
				state = BC_WALKING;
				reset_time = true;
			}
		}
	}
}

void BCTrooper::Walk(float dt)
{
	if (reset_time)
	{
		Change_State.Start();
		reset_time = false;
	}
	else
	{
		if (Change_State.ReadSec() > 1)
		{
			if (ChangeRadius(10, false))
			{
				state = BC_IDLE;
				reset_time = true;
			}
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	{
		position.y -= 3;
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		position.x += 3;
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		position.y += 3;
	}
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		position.x -= 3;
	}
}


void BCTrooper::Hit()
{

}

void BCTrooper::Death()
{
}

bool BCTrooper::ChangeRadius(int radius_to_stop, bool incremenet)
{
	if (incremenet)
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
	else
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

void BCTrooper::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		if (c1 == collision_maze && c2->type == COLLIDER_PLAYER)
		{
			Player* link = (Player*)c1->callback;
			link->hp -= 1;
			link->position.x -= 20;
		}
	}

}




