#pragma once
#ifndef __VILAGERS_H_
#define __VILAGERS_H_

#include "NPC.h"
#include "Pokemon.h"

enum VilagerState { V_IDLE = 0, V_TALKING };

class Vilager : public NPC
{
public:
	Vilager();

	~Vilager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

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
	VilagerState state = V_IDLE;
	VilagerState anim_state = V_IDLE;
	//Item* drop;
	bool stunned = false;
	bool active = false;

	std::string dialog;

};

#endif //__VILAGERS_H_


