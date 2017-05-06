#pragma once
#ifndef __VILAGER_H_
#define __VILAGER_H_

#include "NPC.h"

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

	std::string GetDialog();


private:
	VilagerState state = V_IDLE;
	VilagerState anim_state = V_IDLE;
	//Item* drop;
	bool stunned = false;
	bool active = false;
	int switch_map = 0;

	std::string dialog;

};

#endif //__VILAGER_H_


