#include "BCTrooper.h"
#include "j1Scene.h"
#include "ParticleManager.h"
#include "j1Collision.h"
#include "j1Map.h"
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
	speed = 10;
	speed_bole = 1;
	hp = 100;
	//Get the animations
	animation = *App->anim_manager->GetAnimStruct(BC_TROOPER);
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
			Orientate();
			Idle();
			break;

			//
		case BC_WALKING:
			Orientate();
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

	if (state != BC_HIT && state != BC_DYING)
	{
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
		pos_in_vect += speed_bole;
		if (pos_in_vect >= NUM_POINTS_CIRCLE)
		{
			pos_in_vect = 1;
		}
		bole.x = points[pos_in_vect].x;
		bole.y = points[pos_in_vect].y;
	}

	//Increment dificult
	if (hp < 50)
	{
		speed_bole = 2;
	}
	if (hp < 25)
	{
		speed_bole = 5;
	}
	//Update colliders position
	collision_feet->SetPos(position.x, position.y);
	collision_maze->SetPos(bole.x, bole.y);
	return true;
}

void BCTrooper::Draw()
{
	if (state != BC_HIT && state != BC_DYING)
	{
		//Draw Circle of movement bole
		if (App->collision->Getdebug())
		{
			SDL_Rect temp = { 20,12,2,2 };
			for (int i = 0; i < NUM_POINTS_CIRCLE; i++)
			{
				App->render->Blit(texture, points[i].x, points[i].y, &temp);
			}
		}

		//Draw Chain
		SDL_Rect temp_3 = { 0,0,radius,6 };
		App->render->Blit(texture, position.x, position.y, &temp_3, 1, true, pos_in_vect * MULTI_P, 5, 5);

		//Draw Bole
		SDL_Rect temp_2 = { 0,7,14,14 };
		App->render->Blit(texture, bole.x - 5, bole.y - 4, &temp_2);
	}


	//Draw BCTrooper
	if (direction == UP)
	{
		anim_rect = animation.anim[state].North_action.GetCurrentFrame();
	}
	else if (direction == DOWN)
	{
		anim_rect = animation.anim[state].South_action.GetCurrentFrame();
	}
	else if (direction == LEFT)
	{
		anim_rect = animation.anim[state].West_action.GetCurrentFrame();
	}
	else if (direction == RIGHT)
	{
		anim_rect = animation.anim[state].East_action.GetCurrentFrame();
	}

	App->render->Blit(animation.graphics, position.x, position.y, &anim_rect);


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
		if (Change_State.ReadSec() > 2)
		{
			if (ChangeRadius_degrade(50, true))
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
			if (ChangeRadius_degrade(10, false))
			{
				state = BC_IDLE;
				reset_time = true;
			}
		}
	}
	if (hp > 30) //Relative
	{
		if (canmove % 2 == 0)
		{
			//Move BCTrooper
			Move(dt);
		}
		canmove++;
	}
	else
	{
		//Move BCTrooper
		Move(dt);
	}

}

bool BCTrooper::Move(float dt)
{
	if (direction == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - ceil(speed*dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= ceil(speed*dt);
		}
		walking = true;
	}

	if (direction == RIGHT)
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + ceil(speed*dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += ceil(speed*dt);
		}
		walking = true;
	}
	if (direction == UP)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - ceil(speed*dt), collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			position.y -= ceil(speed*dt);
		}
		walking = true;
	}
	if (direction == DOWN)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + ceil(speed*dt), collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += ceil(speed*dt);
		}
		walking = true;
	}

	return true;
}


void BCTrooper::Hit()
{
	if (reset_time)
	{
		Wait_attack.Start();
		reset_time = false;
	}
	else
	{
		if (Wait_attack.ReadSec() > 1)
		{
			if (ChangeRadius_insta(10, false))
			{
				stunned = false;
				state = BC_IDLE;
				reset_time = true;
			}
		}
	}
}

void BCTrooper::Death()
{
	//40 - 13 // 41 - 13
	App->map->EditCost(40, 13, 0);
	App->map->EditCost(41, 13, 0);
	App->entity_elements->DeleteElement("door");
}

bool BCTrooper::ChangeRadius_degrade(int radius_to_stop, bool incremenet)
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

bool BCTrooper::ChangeRadius_insta(int radius_to_go, bool incremenet)
{
	if (incremenet)
	{
		radius = radius_to_go;
		float factor = (float)M_PI / 180.0f * MULTI_P;
		for (uint i = 0; i < NUM_POINTS_CIRCLE; ++i)
		{
			points[i].x = (int)(position.x + radius * cos(i * factor));
			points[i].y = (int)(position.y + radius * sin(i * factor));
		}
	}
	else
	{
		radius = radius_to_go;
		float factor = (float)M_PI / 180.0f * MULTI_P;
		for (uint i = 0; i < NUM_POINTS_CIRCLE; ++i)
		{
			points[i].x = (int)(position.x + radius * cos(i * factor));
			points[i].y = (int)(position.y + radius * sin(i * factor));
		}
	}
	return true;
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

		if (c1 == collision_feet && c2->type == COLLIDER_SWORD)
		{
			if (stunned == false)
			{
				hp -= 10;
				if (hp > 0)
				{
					state = BC_HIT;
				}
				else
				{
					state = BC_DYING;
				}
				reset_time = true;
				stunned = true;
			}
		}
	}
}




