#pragma once
#ifndef __SOLDIER_H_
#define __SOLDIER_H_

#include "NPC.h"
#include <vector>

enum SoldierType { AGGRESSIVE, PASSIVE };
enum SoldierState { S_IDLE = 0, S_WALKING, S_GUARD, S_DYING, S_ATTACKING, S_CHASING, S_HIT };


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
	bool Update(float dt);

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// Called before quitting
	bool CleanUp();

	void OnCollision(Collider*, Collider*);

	bool CheckPlayerPos();

	//STATE MACHINE ----------
	bool Idle();
	bool Walking(float dt);
	bool Move(float dt);
	bool Chase(float dt);
	bool Attack();
	//Knockback
	bool Movebyhit(float dt);
	bool Die();
	// -----------------------

	SoldierState GetState() const;
	void SetState(SoldierState state);
	void SetAnimState(SoldierState state);

	SoldierType GetType() const;

private:
	SoldierType soldier_type;
	uint id = 0;
	SoldierState state = S_IDLE;
	SoldierState anim_state = S_IDLE;

	//Soldier Test for Release 0.1
	AnimationStruct animation;
	SDL_Rect anim_rect;
	iPoint pivot;
	//Item* drop;
	bool stunned = false;

	int item_id = 0;
	Item* item_inside = nullptr;
	SDL_Texture* texture = nullptr;

	int timetoplay;

	//WHEN ATTACKED
	int timetorun = 0;
	bool reset_run = false;
	int distance = 0;
	int dis_moved = 0;
	bool reset_distance = false;
};

#endif //__SOLDIER_H_