#pragma once
#ifndef __GREENMINION_H_
#define __GREENMINION_H_

#include "NPC.h"

enum GMinionState { GM_WALKING = 0, GM_HIT, GM_DYING, GM_SPAWNING, GM_IDLE };


class GreenMinion : public NPC
{
public:
	GreenMinion();
	~GreenMinion();

	// Called before the first frame
	bool Start(iPoint pos);

	// Called each loop iteration
	bool Update(float dt);

	void Draw();

	// Called before quitting
	//bool CleanUp();

	//void OnCollision(Collider*, Collider*);

	//bool CheckPlayerPos();

	////STATE MACHINE ----------
	bool Spawning();
	bool Idle();
	bool Walking(float dt);
	bool Move(float dt);
	bool Movebyhit(float dt);
	bool Die();
	// -----------------------

private:
	GMinionState state = GM_IDLE;

	//SPAWN VARIABLES ---
	j1Timer spawn_time;
	// -----------------------

	//ANIMATION VARIABLES -------
	GMinionState anim_state = GM_WALKING;
	AnimationStruct animation;
	SDL_Rect anim_rect;
	iPoint pivot;
	// ---------------------------------

	//HIT TIMERS/VARIABLES ---
	int distance = 0;
	int dis_moved = 0;
	// -------------------------

};

#endif
