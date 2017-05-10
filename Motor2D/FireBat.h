#pragma once
#ifndef __FIREBAT_H_
#define __FIREBAT_H_

#include "NPC.h"

enum FireBatState { B_AWAKENING = 0, B_FLYING, B_DYING, B_SLEEP };


class FireBat : public NPC
{
public:
	FireBat();
	~FireBat();

	// Called before the first frame
	bool Start(iPoint* path);

	// Called each loop iteration
	bool Update(float dt);

	void Draw();

private:

	//POSITION & MOVEMENT VARIABLES ---
	iPoint* path = nullptr;
	// -------------

	FireBatState state = B_SLEEP;

	//SPAWN VARIABLES ---
	j1Timer spawn_time;
	// -----------------------

	//ANIMATION VARIABLES -------
	FireBatState anim_state = B_SLEEP;
	AnimationStruct animation;
	SDL_Rect anim_rect;
	iPoint pivot;
	// ---------------------------------


};

#endif
