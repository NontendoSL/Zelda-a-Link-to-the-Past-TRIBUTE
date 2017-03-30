#pragma once
#ifndef __ITEMS_H_
#define __ITEMS_H_

#include "SceneElements.h"


enum itemtype{ ZELDA_W, POKEMON_W };
//enum ItemID { NO_ITEM, RUPEE, BOMB, ARROW, BOW, HOOKSHOT };

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
	//bool Update();

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// Called before quitting
	bool CleanUp();

	bool Save();

private:
	SDL_Texture* texture = nullptr;

public:
	Collider* collision = nullptr;
	//bool picked = false;
	bool equiped = false;
	bool equipable = false;
};


class Hookshot :public Item
{
public:
	Hookshot(bool equipable);
	~Hookshot();

	bool Update();

	void SetRange(uint charge);

	//Check the tile at its position each X Updates
	bool ReachObjective();

private:
	uint range = 0;
	//std::vector<iPoint

};

#endif