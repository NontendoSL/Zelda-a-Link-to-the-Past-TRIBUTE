#pragma once
#ifndef __POKEMON_H_
#define __POKEMON_H_

#include "NPC.h"
#include "j1Window.h"
#include "j1Collision.h"

class Pokemon : public NPC
{
public:
	Pokemon()
	{}

	~Pokemon()
	{}

	/*// Called before render is available
	bool Awake(pugi::xml_node&, uint)
	{
		return true;
	}

	// Called before the first frame
	bool Start()
	{
		return true;
	}

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update()
	{
		return true;
	}

	// Called before all Updates
	//bool PostUpdate();

	void Draw()
	{}

	// Called before quitting
	bool CleanUp()
	{
		return true;
	}

	void AddItem(Item*)
	{}

	void Drop_item()
	{}*/

	iPoint Getposition() const
	{
		return position;
	}

public:

	bool active = false;
	bool dead = false;
	bool pokemon_player = false;
	uint cooldown;
	//hits
	Direction dir_hit;
	iPoint previus_position;
	j1Timer knockback_time;

	//target
	Pokemon* target = nullptr;

protected:

	//Item* drop;
	Item* item_inside= nullptr;

	Animation* current_animation = nullptr;

	bool attacker = false;
	bool getdamage = false;

	//special attack
	iPoint range; //.X shows max range, .Y shows distance moved
	iPoint sp_start;
	bool sp_attacking = false;
	int sp_direction = 0;
	int sp_speed = 0;
	uint sp_damage;
	Collider* sp_attack = nullptr;

	j1Timer time_wait;


	Collider* collision_attack = nullptr;
};

#endif //__POKEMON_H_