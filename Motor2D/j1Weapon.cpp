#include "j1Weapon.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "Pokemon.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1Map.h"

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

	return true;
}

void Hookshot::SetRange(float charge)
{
	range = charge * 1.5;
}

HookState Hookshot::ReachObjective(int actual_floor)
{
	//TODO HIGH -> made it functional to Zelda World
	const MapLayer* meta_layer = App->map->data.layers[2]; // metasudo layer
	iPoint map_pos = App->map->WorldToMap(position.x, position.y);
	int hook_tile = meta_layer->Get(map_pos.x, map_pos.y);

	const TileSet* tileset = App->map->data.tilesets[1];

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
	if (in_use == true)
	{
		App->anim_manager->Drawing_Manager(anim_state, direction, position, HOOKSHOT); //id 2 = hookshot animation xml
	}
}


/*{
switch (player_level)
{
case LEVEL_1:
if (orange tile touched)
{
return wall;
}
break;
case LEVEL_2:
if (yellow tile touched)
{
return wall;
}
break;
case LEVEL_3:
if (pink tile touched)
{
return wall;
}
break;
default:
break;
}

if (sudowoodo touched)
{
return true;
}
else
{
return false;
}*/

// -------------------


//HOOKSHOT ------------------
Bow::Bow(bool equip)
{
	equipable = equip;
}


Bow::~Bow()
{
}

bool Bow::Start()
{
	return true;
}

bool Bow::Update(float dt)
{

	return true;
}

void Bow::SetSpeed(uint charge)
{
	arrow_speed = charge / 10;
}

/*void Bow::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		if (c1->type == COLLIDER_ARROW && c2->type == COLLIDER_ENEMY)
		{

		}

}*/



/*{
switch (player_level)
{
case LEVEL_1:
if (orange tile touched)
{
return wall;
}
break;
case LEVEL_2:
if (yellow tile touched)
{
return wall;
}
break;
case LEVEL_3:
if (pink tile touched)
{
return wall;
}
break;
default:
break;
}

if (sudowoodo touched)
{
return true;
}
else
{
return false;
}*/

// -------------------
//BOMBContainer

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



//--------------------------------- BOMB: TODO HIGH -> modify animation management.
Bomb::Bomb(iPoint position, BombContainer*container) : position(position), container(container)
{
	radius = 20;
	timer = SDL_GetTicks();
	step = PLANTED;
}

Bomb::~Bomb()
{

}

void Bomb::Update(float dt)
{
	if (SDL_GetTicks() > timer + 2000 && step != EXPLOSION)
	{
		App->audio->PlayFx(7);
		step = EXPLOSION;
		collision = App->collision->AddCollider({ position.x - radius,position.y - radius,radius * 2,radius * 2 }, COLLIDER_BOMB);
		current = App->anim_manager->GetAnimation((WeaponState)2, DOWN, BOMB);
		current->Reset();
	}
	if (step == EXPLOSION && current->Finished())
	{
		Die();
	}
}

void Bomb::Draw()
{
	switch (step) {
	case PLANTED:
		App->anim_manager->Drawing_Manager((WeaponState)1, DOWN, position, BOMB);
		break;
	case EXPLOSION:
		App->anim_manager->Drawing_Manager((WeaponState)2, DOWN, position, BOMB);
		break;
	}
}

void Bomb::Die()
{
	collision->to_delete = true;
	container->CleanContainer();
}

