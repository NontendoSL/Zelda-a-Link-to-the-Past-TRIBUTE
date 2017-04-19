#pragma once
#ifndef __POKEMON_H_
#define __POKEMON_H_

#include "NPC.h"
#include "j1Window.h"
#include "j1Collision.h"

enum PokemonState { P_IDLE = 0, P_WALKING, P_ATTACKING, P_HIT, P_DYING, P_SPECIAL };


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
	}*/

	iPoint Getposition() const
	{
		return position;
	}

public:

	bool active = false;
	bool dead = false;
	bool pokemon_player = false;
	uint cooldown;

	//target
	Pokemon* target = nullptr;

	//special attack
	iPoint range; //.X shows max range, .Y shows distance moved
	iPoint sp_start;
	bool sp_attacking = false;
	int sp_direction = 0;
	int sp_speed = 0;
	uint sp_damage;
	Collider* sp_attack = nullptr;

	Collider* collision_attack = nullptr;

protected:
	//Item* drop;
	Item* item_inside= nullptr;

	PokemonState state = P_IDLE;
	PokemonState anim_state = P_IDLE;

	Animation* current_animation = nullptr;

	bool attacker = false;
	bool getdamage = false;

	j1Timer time_wait;

};

#endif //__POKEMON_H_