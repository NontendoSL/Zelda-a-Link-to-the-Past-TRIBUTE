#include "j1Weapon.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "Pokemon.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "Soldier.h"
#include "ParticleManager.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1App.h"

Weapon::Weapon() :SceneElement()
{
	name = "weapons";
	type = WEAPON;
}

Weapon::~Weapon(){}

bool Weapon::Awake(pugi::xml_node &conf, uint id)
{
	bool stop_search = false;
	for (int s_id = conf.child("weapon").attribute("id").as_int(0); stop_search == false; s_id = conf.child("weapon").next_sibling().attribute("id").as_int(0))
	{
		if (id == s_id)
		{
			name = conf.child("weapon").attribute("name").as_string("");
			position.x = 0;
			position.y = 0; 
			stop_search = true;
		}
	}
	return true;
}

//HOOKSHOT ------------------
Hookshot::Hookshot(bool equip)
{
	equipable = equip;
	Wtype = W_HOOKSHOT;
}


Hookshot::~Hookshot()
{
}

bool Hookshot::Start()
{
	speed = 3;
	state = W_IDLE;
	anim_state = W_IDLE;

	return true;
}

bool Hookshot::Update(float dt)
{
	BROFILER_CATEGORY("Update_Collision", Profiler::Color::Aqua);
	return true;
}

void Hookshot::SetRange(float charge)
{
	range = charge * 1.5;
}

HookState Hookshot::ReachObjective(int actual_floor)
{
	//TODO HIGH -> made it functional to Zelda World
	const MapLayer* meta_layer = App->map->data.layers[App->map->data.layers.size() - 1]; //WEAPONS LAYER
	iPoint map_pos = App->map->WorldToMap(position.x, position.y);
	int hook_tile = meta_layer->Get(map_pos.x, map_pos.y);

	const TileSet* tileset = App->map->data.tilesets[0]; // META TILESET

	int first_floor_hook = tileset->firstgid + 7; // PINK TILE
	int second_floor_hook = tileset->firstgid + 9; // BLACK TILE
	int third_floor_hook = tileset->firstgid + 6; // ORANGE TILE
	int target = tileset->firstgid + 8;

	if (actual_floor == 0)
	{
		if (hook_tile == first_floor_hook)
		{
			target_reached = false;
			hook_state = OBSTACLE;
			return hook_state;
		}
	}
	else if (actual_floor == 1)
	{
		if (hook_tile == second_floor_hook)
		{
			target_reached = false;
			hook_state = OBSTACLE;
			return hook_state;
		}
	}
	else if (actual_floor == 2)
	{
		if (hook_tile == third_floor_hook)
		{
			target_reached = false;
			hook_state = OBSTACLE;
			return hook_state;
		}
	}

	if (hook_tile == target) //hookchange
	{
		target_reached = true;
		hook_state = TARGET;
		return hook_state;
	}
	else
	{
		target_reached = false;
		hook_state = MISS;
		return hook_state;
	}
}

HookState Hookshot::GetState()
{
	return hook_state;
}

void Hookshot::Reset()
{
	target_reached = false;
	actual_range_pos = 0;
	range = 0;
	collision->to_delete = true;
	hook_state = MISS;
	in_use = false;
}

void Hookshot::Draw()
{
	BROFILER_CATEGORY("Draw_Weapon", Profiler::Color::Beige);
	if (in_use == true)
	{
		App->anim_manager->Drawing_Manager(anim_state, direction, position, HOOKSHOT); //id 2 = hookshot animation xml
	}
}
// ----------------------------------

// BOW ------------------------------
Bow::Bow(bool equip)
{
	equipable = equip;
	Wtype = W_BOW;
}

Bow::~Bow()
{
}

bool Bow::Start()
{
	arrow_speed = 7;
	state = W_IDLE;
	anim_state = W_IDLE;
	return true;
}

bool Bow::Update(float dt)
{
	BROFILER_CATEGORY("Update_Collision", Profiler::Color::Bisque);
	
	if (arrows.size() > 0)
	{
		std::list<Arrow*>::const_iterator item = arrows.begin();
		while (item != arrows.end())
		{
			item._Ptr->_Myval->Update(dt);
			item++;
		}
	}
	return true;
}

void Bow::Draw()
{
	std::list<Arrow*>::const_iterator item = arrows.begin();
	while (item != arrows.end())
	{
		item._Ptr->_Myval->Draw();
		item++;
	}
}

void Bow::CleanContainer()
{
	arrows.pop_front();
}



float Bow::SetSpeed(float charge)
{
	return arrow_speed * charge; //More charge = more speed = more distance
}

void Bow::Shoot(iPoint pos, Direction dir, float charge)
{
	App->audio->PlayFx(18);
	float speed = SetSpeed(charge);
	Arrow* arrow = new Arrow(pos, dir, this, speed);

	if (dir == UP || dir == DOWN)
	{
		arrow->offset_x = 3;
		arrow->offset_y = 7;
		arrow->collision = App->collision->AddCollider({ arrow->position.x - arrow->offset_x, arrow->position.y - arrow->offset_y, 7, 15 }, COLLIDER_ARROW, nullptr, arrow);
	}

	else if (dir == LEFT || dir == RIGHT)
	{
		arrow->offset_x = 7;
		arrow->offset_y = 3;
		arrow->collision = App->collision->AddCollider({ arrow->position.x - arrow->offset_x, arrow->position.y - arrow->offset_y, 15, 7 }, COLLIDER_ARROW, nullptr, arrow);
	}
	
	arrows.push_back(arrow);
}

// ARROW -------------------------------------------------------
Arrow::Arrow(iPoint pos, Direction dir, Bow* container, float speed):
	direction(dir), container(container),arrow_speed(speed)
{
	if (direction == LEFT || direction == RIGHT)
	{
		position.x = pos.x;
		position.y = pos.y - 6;
	}
	else
	{
		position = pos;
	}

	lifetime = 1; //Seconds before disappearing.
	step = AIR;
	App->particlemanager->CreateFollow_P(nullptr, &position, { 0,2,2,0 }, { 2,2 }, { 30,15 }, 4, 10, true);
	particles = App->particlemanager->Group_Follow.back();
	timer.Start();
}

Arrow::~Arrow()
{
}

void Arrow::Update(float dt)
{
	if (App->scene->gamestate == INGAME)
	{
		if (timer.ReadSec() >= lifetime && step != DIE)
		{
			step = DIE;
		}

		if (step == AIR)
		{
			KeepGoing(dt);

			//Set collision to the arrow.
			collision->SetPos(position.x - offset_x, position.y - offset_y);

			//Check for a WALL IMPACT.
			if (IsImpact(App->scene->player->GetFloor()) == WALL_IMPACT)
			{
				impact_time.Start();
				App->audio->PlayFx(17);
			}
		}

		else if (step == WALL_IMPACT)
		{
			if (impact_time.ReadSec() >= 0.5)
			{
				step = DIE;
			}
		}

		else if (step == ENEMY_IMPACT)
		{
			if (start_impact == true)
			{
				current = App->anim_manager->GetAnimation(ENEMY_IMPACT, direction, ARROW);
				current->Reset();
				start_impact = false;
			}
			else if (current->Finished())
			{
				step = DIE;
			}
		}

		else if (step == DIE)
		{
			Die();
		}
	}
}

void Arrow::Draw()
{
	switch (step)
	{
	case AIR:
		App->anim_manager->Drawing_Manager(AIR, direction, position, ARROW);
		break;
	case WALL_IMPACT:
		App->anim_manager->Drawing_Manager(WALL_IMPACT, direction, position, ARROW);
		break;
	case ENEMY_IMPACT:
		App->anim_manager->Drawing_Manager(ENEMY_IMPACT, direction, position, ARROW);		
		break;
	default:
		break;
	}
}

void Arrow::KeepGoing(float dt)
{
	switch (direction)
	{
	case UP:
		position.y -= ceil(arrow_speed*dt);
		break;
	case DOWN:
		position.y += ceil(arrow_speed*dt);
		break;
	case LEFT:
		position.x -= ceil(arrow_speed*dt);
		break;
	case RIGHT:
		position.x += ceil(arrow_speed*dt);
		break;
	default:
		break;
	}
}

ArrowStep Arrow::IsImpact(int actual_floor)
{
	const MapLayer* meta_layer = App->map->data.layers[App->map->data.layers.size() - 1]; //WEAPONS LAYER
	iPoint map_pos = App->map->WorldToMap(position.x, position.y);
	int arrow_tile = meta_layer->Get(map_pos.x, map_pos.y);

	const TileSet* tileset = App->map->data.tilesets[0]; // META TILESET

	int first_floor = tileset->firstgid + 7; // PINK TILE
	int second_floor = tileset->firstgid + 9; // BLACK TILE
	int third_floor = tileset->firstgid + 6; // ORANGE TILE

	if (actual_floor == 0)
	{
		if (arrow_tile == first_floor)
		{
			step = WALL_IMPACT;
		}
	}
	else if (actual_floor == 1)
	{
		if (arrow_tile == second_floor)
		{
			step = WALL_IMPACT;
		}
	}
	else if (actual_floor == 2)
	{
		if (arrow_tile == third_floor)
		{
			step = WALL_IMPACT;
		}
	}
	else
	{
		step = AIR;
	}

	return step;
}

void Arrow::Die()
{
	collision->to_delete = true;
	container->CleanContainer();
	App->particlemanager->DeleteFollow_p(particles);
}


//-------------------------------------
//-------------------------------------------------


//BOMB CONTAINER -------------------------------
BombContainer::BombContainer()
{
	equipable = true;
	Wtype = W_BOMB;
}

BombContainer::~BombContainer(){}

void BombContainer::Drop(iPoint position)
{
	bombs.push_back(new Bomb(position, this));
}

bool BombContainer::Update(float dt)
{
	std::list<Bomb*>::iterator item = bombs.begin();
	while (item != bombs.end())
	{
		item._Ptr->_Myval->Update(dt);
		item++;
	}
	return true;
}

void BombContainer::Draw()
{
	std::list<Bomb*>::iterator item = bombs.begin();
	while (item != bombs.end())
	{
		item._Ptr->_Myval->Draw();
		item++;
	}
}

void BombContainer::CleanContainer()
{
	bombs.pop_front();
}


// BOMB ----------------------------------------  TODO HIGH -> modify animation management.
Bomb::Bomb(iPoint position, BombContainer*container) : position(position), container(container)
{
	radius = 20;
	timer.Start();
	step = PLANTED;
}

Bomb::~Bomb()
{

}

void Bomb::Update(float dt)
{
	if (App->scene->gamestate == INGAME)
	{
		if (timer.ReadSec() >= 2.0f && step != EXPLOSION)
		{
			App->audio->PlayFx(7);
			step = EXPLOSION;
			collision = App->collision->AddCollider({ position.x - radius,position.y - radius,radius * 2,radius * 2 }, COLLIDER_BOMB);
			current = App->anim_manager->GetAnimation(W_DYING, DOWN, BOMB);
			current->Reset();
		}

		if (step == EXPLOSION && current->Finished())
		{
			Die();
		}
	}
}

void Bomb::Draw()
{
	switch (step) 
	{
	case PLANTED:
		App->anim_manager->Drawing_Manager(W_IDLE, DOWN, position, BOMB);
		break;
	case EXPLOSION:
		App->anim_manager->Drawing_Manager(W_DYING, DOWN, position, BOMB);
		break;
	}
}

void Bomb::Die()
{
	collision->to_delete = true;
	container->CleanContainer();
}

//------------------------------------
//----------------------------------------------------

