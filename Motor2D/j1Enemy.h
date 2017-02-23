#pragma once
#ifndef __ENEMIES_H_
#define __ENEMIES_H_

#include "j1SceneElements.h"
class Item;

class Enemy : public j1SceneElement
{
public:
	Enemy(iPoint position);

	~Enemy();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update();

	// Called before all Updates
	//bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Save();

	void AddItem(Item*);

	void Drop_item();

private:
	int attack;
	//Item* drop;
	float speed;
	bool stunned=false;
	Item* item_inside;
	SDL_Texture* texture;
};

#endif