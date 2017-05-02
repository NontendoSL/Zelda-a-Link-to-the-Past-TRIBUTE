#include "BCTrooper.h"
#include "j1Scene.h"
#include "ParticleManager.h"

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

	float factor = (float)M_PI / 180.0f;
	for (uint i = 0; i < 360; ++i)
	{
		iPoint temp;
		temp.x = (int)(100 + radius * cos(i * factor));
		temp.y = (int)(100 + radius * sin(i * factor));
		points.push_back(temp);
	}
	bole.x = points.begin()._Ptr->x;
	bole.y = points.begin()._Ptr->y;
	position.x = 100;
	position.y = 100;
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

	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
	{
		radius += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	{
		radius -= 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		speed += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		speed -= 1;
	}

	//
	pos_in_vect += speed;
	if (pos_in_vect >= 360)
	{
		pos_in_vect = 1;
	}
	bole.x = points[pos_in_vect].x;
	bole.y = points[pos_in_vect].y;
	return true;
}

void BCTrooper::Draw()
{
	/*SDL_Rect temp = { 20,12,2,2 };
	for (int i = 0; i < 360; i++)
	{
		App->render->Blit(texture, points[i].x, points[i].y, &temp);
	}*/

	SDL_Rect temp_3 = { 0,0,radius,6 };
	App->render->Blit(texture, position.x, position.y, &temp_3, 0, true, pos_in_vect, 5, 5);
	

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
		if (Change_State.ReadSec() > 3)
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
		if (Change_State.ReadSec() > 5)
		{
			if (ChangeRadius(10, false))
			{
				state = BC_IDLE;
				reset_time = true;
			}
		}
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
		float factor = (float)M_PI / 180.0f;
		for (uint i = 0; i < 360; ++i)
		{
			points[i].x = (int)(100 + radius * cos(i * factor));
			points[i].y = (int)(100 + radius * sin(i * factor));
		}
		if (radius == radius_to_stop)
		{
			return true;
		}
	}
	else
	{
		radius -= 1;
		float factor = (float)M_PI / 180.0f;
		for (uint i = 0; i < 360; ++i)
		{
			points[i].x = (int)(100 + radius * cos(i * factor));
			points[i].y = (int)(100 + radius * sin(i * factor));
		}
		if (radius == radius_to_stop)
		{
			return true;
		}
	}
	return false;
}




