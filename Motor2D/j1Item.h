#pragma once
#ifndef __ITEMS_H_
#define __ITEMS_H_

#include "SceneElements.h"


enum itemtype{ ZELDA_W, POKEMON_W };

class Item : public SceneElement
{
public:
	Item();

	~Item();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint, iPoint );

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update();

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// Called before quitting
	bool CleanUp();

	bool Save();

private:
	bool picked = false;
	bool equiped = false;
	bool equipable = false;
	SDL_Texture* texture = nullptr;
public:
	Collider* collision = nullptr;
};


/*class Hookshot :public Item
{
public:
	Hookshot();
	~Hookshot();

	//Check the tile at its position each X Updates
	bool ReachObjective();

private:
	uint range = 0;

};*/

#endif