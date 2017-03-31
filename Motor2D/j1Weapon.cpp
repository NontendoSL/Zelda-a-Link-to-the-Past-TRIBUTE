#include "j1Weapon.h"
#include "j1Textures.h"
#include "j1Render.h"
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
}


Hookshot::~Hookshot()
{
}

bool Hookshot::Start()
{
	speed = 2;
	state = IDLE;
	return true;
}

bool Hookshot::Update()
{

	return true;
}

void Hookshot::SetRange(uint charge)
{
	range = charge;
}

HookState Hookshot::ReachObjective()
{
	if (App->map->MovementCost(position.x, position.y, direction) == T_WALL)
	{
		target_reached = true;
		hook_state = TARGET;
	}
	else
	{
		target_reached = false;
		hook_state = MISS;
	}

	return hook_state;
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
		App->anim_manager->Drawing_Manager(state, direction, position, 1); //id 1 = hookshot animation xml
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

bool Bow::Update()
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
