#pragma once
#ifndef __SOLDIER_H_
#define __SOLDIER_H_

#include "NPC.h"

class Soldier : public NPC
{
public:
	Soldier(iPoint position);

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

	//Item* drop;
	bool stunned=false;
	Item* item_inside;
	SDL_Texture* texture;
	Collider* collision_enemy;
};

#endif //__SOLDIER_H_