#pragma once
#ifndef __BCTROOPER_H_
#define __BCTROOPER_H_

#include "NPC.h"

class BCTrooper : public NPC
{
public:
	BCTrooper();

	~BCTrooper();

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
	bool stunned = false;
	Item* item_inside = nullptr;
	SDL_Texture* texture = nullptr;
	Collider* collision_feet = nullptr;
};

#endif//__BCTROOPER_H_
