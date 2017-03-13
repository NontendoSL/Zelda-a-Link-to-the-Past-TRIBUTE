#pragma once
#ifndef __SOLDIER_H_
#define __SOLDIER_H_

#include "NPC.h"

class Soldier : public NPC
{
public:
	Soldier();

	~Soldier();

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

	void Draw();

	// Called before quitting
	bool CleanUp();

	void AddItem(Item*);

	void Drop_item();

private:

	//Soldier Test for Release 0.1
	SDL_Rect soldier_left;
	SDL_Rect soldier_right;
	SDL_Rect soldier_up;
	SDL_Rect soldier_down;

	SDL_Rect soldier_left_2;
	SDL_Rect soldier_right_2;
	SDL_Rect soldier_up_2;
	SDL_Rect soldier_down_2;
	//Item* drop;
	bool stunned=false;
	Item* item_inside;
	SDL_Texture* texture;
	Collider* collision_enemy;
};

#endif //__SOLDIER_H_