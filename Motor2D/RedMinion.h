#pragma once
#ifndef __REDMINION_H_
#define __REDMINION_H_

#include "NPC.h"

enum RMinionState { RM_WALKING = 0, RM_DYING, RM_HIT, RM_SPAWNING, RM_IDLE };


class RedMinion : public NPC
{
public:
	RedMinion();
	~RedMinion();

	// Called before the first frame
	bool Start(iPoint pos);

	// Called each loop iteration
	bool Update(float dt);

	void Draw();

	// Called before quitting
	//bool CleanUp();

	void OnCollision(Collider*, Collider*);

	//bool CheckPlayerPos();

	////STATE MACHINE ----------
	bool Spawning();
	bool Idle();
	bool Walking(float dt);
	bool Move(float dt);
	bool Movebyhit(float dt);
	bool Die();
	// -----------------------

	// UTILITY FUNCTIONS ---------
	void SetKnockbackDir();
	int DropItem();
	void SetState(RMinionState state);
	void SetAnimState(RMinionState state);
	RMinionState GetState() const;
	//----------------------------

	AnimationStruct explosion_anim;

private:
	RMinionState state = RM_IDLE;

	//SPAWN VARIABLES ---
	j1Timer spawn_time;
	// -----------------------

	//ANIMATION VARIABLES -------
	RMinionState anim_state = RM_WALKING;
	AnimationStruct animation;
	SDL_Rect anim_rect;
	iPoint pivot;
	SDL_Texture* death_graphics = nullptr;
	// ---------------------------------

	//MOVEMENT VARIABLES ---
	int distance = 0;
	int dis_moved = 0;
	int canmove = 0;
	// -------------------------


};

#endif
