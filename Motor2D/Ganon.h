#pragma once
#ifndef __GANON_H_
#define __GANON_H_

#include "NPC.h"

enum GanonState { G_IDLE = 0, G_WALKING, G_MELEE, G_SPECIAL, G_SHIELD, G_DYING };

class Ganon : public NPC
{
public:
	Ganon();

	~Ganon();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// Called before quitting
	bool CleanUp();

private:
	GanonState state = G_IDLE;
	GanonState anim_state = G_IDLE;

	//Item* drop;
	bool stunned = false;
	Item* item_inside = nullptr;
	SDL_Texture* texture = nullptr;
};

#endif //__GANON_H_