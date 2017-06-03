#include "BCTrooper.h"
#include "j1Scene.h"
#include "ParticleManager.h"
#include "P_Follow.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Weapon.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1DynamicObjects.h"

BCTrooper::BCTrooper() : NPC()
{
	name = "BCTrooper";
	type = CREATURE;
}

BCTrooper::~BCTrooper()
{
	points.clear();
	App->tex->UnLoad(texture);
	texture = nullptr;
	for (int i = 0; i < 4; i++)
	{
		boles[i].particle_maze = nullptr;
		boles[i].collision_maze = nullptr;
	}
}

bool BCTrooper::Awake(pugi::xml_node &conf, uint id)
{

	return true;
}

bool BCTrooper::Start()
{
	App->gui->GetEntity("boss bar")->visible = true;
	App->gui->GetEntity("hp boss")->visible = true;
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
	
	for (int i = 0, pos = 0; i < 4; i++, pos += 22)
	{
		Bole temp;
		temp.position = points[pos];
		temp.pos_in_vect = pos;
		temp.active = true;
		boles.push_back(temp);
	}
	// Create Particles
	for (int i = 0; i < 4; i++)
	{
		App->particlemanager->CreateFollow_P(nullptr, &boles[i].position, SDL_Rect{ 0,10,2,0 }, iPoint(5, 5), iPoint(18, 8), 4, 29, false);
		boles[i].particle_maze = App->particlemanager->Group_Follow.back();
	}
	boles[0].particle_maze->active = true;
	//Only active 1 Maze
	for (int i = 1; i < 4; i++)
	{
		boles[i].active = false;
	}
	prev_position = position;
	Wait_attack.Start();
	speed = 10;
	speed_bole = 1;
	hp = 60;
	//Get the animations
	animation = *App->anim_manager->GetAnimStruct(BC_TROOPER);
	collision_feet = App->collision->AddCollider({ position.x - 8, position.y - 5, 16, 15 }, COLLIDER_BCTROOPER, this);
	boles[0].collision_maze = App->collision->AddCollider({ boles[0].position.x - 7, boles[0].position.y - 7, 14, 14 }, COLLIDER_BCTROOPER_MAZE, this);
	return true;
}

bool BCTrooper::Update(float dt)
{
	App->gui->GetEntity("hp boss")->Hitbox.w = (hp * 149) / 60;
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
		case BC_DEFEND:
			Defend();
			break;

			//
		case BC_DYING:
			Death();
			break;

		default:
			break;
		}
	}

	if (hp < 40 && next_phase == false)
	{
		for (int i = 1; i < 4; i++)
		{
			boles[i].active = true;
			boles[i].collision_maze = App->collision->AddCollider({ boles[i].position.x - 7, boles[i].position.y - 7, 14, 14 }, COLLIDER_BCTROOPER_MAZE, this);
		}
		next_phase = true;
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

		for (int i = 0; i < boles.size(); i++)
		{
			boles[i].pos_in_vect += speed_bole;
			if (boles[i].pos_in_vect > NUM_POINTS_CIRCLE - 1)
			{
				int reajust = boles[i].pos_in_vect - (NUM_POINTS_CIRCLE - 1);
				boles[i].pos_in_vect = reajust;
			}

			boles[i].position.x = points[boles[i].pos_in_vect].x;
			boles[i].position.y = points[boles[i].pos_in_vect].y;
		}
	}

	//Increment dificult
	if (hp < 40)
	{
		//speed_bole = 2;
	}
	if (hp < 20)
	{
		//speed_bole = 2;
	}
	//Update colliders position
	collision_feet->SetPos(position.x - 8, position.y - 5);
	for (int i = 0; i < boles.size(); i++)
	{
		if (boles[i].active)
		{
			boles[i].collision_maze->SetPos(boles[i].position.x - 7, boles[i].position.y - 7);
		}
	}

	return true;
}

void BCTrooper::Draw()
{
	BROFILER_CATEGORY("Draw_SOLDIER", Profiler::Color::DarkSlateBlue);

	if (state != BC_DYING)
	{
		if (state == BC_DEFEND)
		{
			App->anim_manager->Drawing_Manager(BC_IDLE, direction, position, BC_TROOPER);
		}
		else
		{
			App->anim_manager->Drawing_Manager(state, direction, position, BC_TROOPER);
		}

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
			for (int i = 0; i < boles.size(); i++)
			{
				if (boles[i].active)
				{
					//Draw Chain
					SDL_Rect chain_r = { 0,0,radius,6 };
					App->render->Blit(texture, position.x, position.y, &chain_r, 1, true, boles[i].pos_in_vect * MULTI_P, 5, 5);

					//Draw Bole
					SDL_Rect bole_r = { 0,7,14,14 };
					App->render->Blit(texture, boles[i].position.x - 5, boles[i].position.y - 4, &bole_r);
				}
			}
			for (int i = 0; i < 4; i++)
			{
				if (boles[i].active)
				{
					boles[i].particle_maze->active = true;
				}
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (boles[i].active)
				{
					boles[i].particle_maze->active = false;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (boles[i].active)
			{
				boles[i].particle_maze->active = false;
			}
		}
	}
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
	if (hp > 20) //Relative
	{
		if (canmove % 2 == 0)
		{
			//Move BCTrooper
			Move(dt);
		}
		if (canmove > 500)
		{
			canmove = 0;
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
		Change_State.Start();
		reset_time = false;
	}
	else
	{
		if (Change_State.ReadSec() < 0.2)
		{
			Movebyhit(2);
		}
		if (Change_State.ReadSec() > 1.5)
		{
			if (ChangeRadius_insta(10, false))
			{
				state = BC_DEFEND;
				reset_time = true;
			}
		}
	}
}

void BCTrooper::Defend()
{
	if (reset_time)
	{
		Change_State.Start();
		reset_time = false;
		save_speed = speed_bole;
		speed_bole = 4;
	}
	else
	{
		if (Change_State.ReadSec() > 1)
		{
			stunned = false;
			state = BC_IDLE;
			reset_time = true;
			speed_bole = save_speed;
		}
	}
}

void BCTrooper::Death()
{
	App->gui->GetEntity("hp boss")->Hitbox.w = 149;
	App->gui->GetEntity("boss bar")->visible = false;
	App->gui->GetEntity("hp boss")->visible = false;
	//40 - 13 // 41 - 13
	App->map->EditCost(40, 13, 0);
	App->map->EditCost(41, 13, 0);
	App->entity_elements->DeleteElement("door");

	App->entity_elements->CreateItem(8, position);

	to_delete = true;
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

BCTrooperState BCTrooper::GetState() const
{
	return state;
}

Collider* BCTrooper::GetColliderMaze(uint pos)
{
	return boles[pos].collision_maze;
}

bool BCTrooper::Movebyhit(int speed)
{
	if (dir_hit == UP)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - 4, collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			position.y -= speed;
		}
	}
	else if (dir_hit == DOWN)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + 4, collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += speed;
		}
	}
	else if (dir_hit == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - 4, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x -= speed;
		}
	}
	else if (dir_hit == RIGHT)
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + 4, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += speed;
		}
	}
	return true;
}

uint BCTrooper::GetMazeSize() const
{
	return boles.size();
}

void BCTrooper::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		if ((c1 == boles[0].collision_maze || c1 == boles[1].collision_maze ||
			c1 == boles[2].collision_maze || c1 == boles[3].collision_maze)	&& c2->type == COLLIDER_PLAYER)
		{
			Player* link = (Player*)c2->callback;
			if (link->invincible_timer.ReadSec() >= 1 && state != BC_HIT && hp > 0)
			{
				App->audio->PlayFx(13);
				link->SetState(L_HIT);
				link->SetAnimState(L_IDLE);
				link->hurt_timer.Start();
				link->invincible_timer.Start();
				link->GetDamage();
				link->dir_hit = direction;
				link->prev_position = position;

				if (link->picked_object != nullptr) // Destroy the picked object if an enemy attacks you.
				{
					link->picked_object->SetState(D_IMPACTING);
					link->picked_object->start_impact = true;
					link->picked_object = nullptr;
				}
			}
		}

		//SWORD COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_SWORD)
		{
			Player* link = (Player*)c2->callback;
			if (Wait_attack.ReadSec() > 1.6 && state != BC_DEFEND)
			{
				App->audio->PlayFx(12);
				hp -= 10;
				state = BC_HIT;
				dir_hit = link->direction;

				reset_time = true;
				stunned = true;
				if (hp <= 0)
				{
					state = BC_DYING;
					App->audio->PlayMusic("audio/music/ZELDA/ZeldaPrincessRescue.ogg", 0); //TODO HIGH -> FIX FPS DROP
				}
				Wait_attack.Start();
			}

		}

		// ARROW COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_ARROW)
		{
			if (stunned == false)
			{
				App->audio->PlayFx(19);
				state = BC_HIT;
				reset_time = true;
				stunned = true;
				c2->arrow_callback->step = ENEMY_IMPACT;
			}
		}

		//DYNOBJECT COLLISION
		if (c1 == collision_feet && c2->type == COLLIDER_DYNOBJECT && c2->callback != nullptr)
		{
			if (((DynamicObjects*)c2->callback)->GetState() == D_AIR)
			{
				App->audio->PlayFx(19);
				state = BC_HIT;
				reset_time = true;
				stunned = true;
				((DynamicObjects*)c2->callback)->SetState(D_IMPACTING);
				((DynamicObjects*)c2->callback)->start_impact = true;
			}
		}
	}
}




