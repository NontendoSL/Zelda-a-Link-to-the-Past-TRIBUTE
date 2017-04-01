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

	// Called before render is available
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
	{}

	iPoint Getposition() const
	{
		return position;
	}

public:

	bool active;
	bool dead;
	bool pokemon_player;

protected:

	//Item* drop;
	Item* item_inside= nullptr;
	Pokemon* target = nullptr;
	Animation* current_animation = nullptr;

	bool attacker = false;

	Collider* collision_attack = nullptr;
};

#endif //__POKEMON_H_