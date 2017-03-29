#pragma once
#ifndef __POKEMON_H_
#define __POKEMON_H_

#include "NPC.h"

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

private:

	//Item* drop;
	Item* item_inside;
	SDL_Texture* texture;
};

#endif //__POKEMON_H_