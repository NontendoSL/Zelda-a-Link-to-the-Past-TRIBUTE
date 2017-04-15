#pragma once
#ifndef __SOLDIER_H_
#define __SOLDIER_H_

#include "NPC.h"
#include <vector>

enum SoldierType {AGGRESSIVE, PASSIVE};

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

	//void OnCollision(Collider* c1, Collider* c2);

	void Draw();

	// Called before quitting
	bool CleanUp();

	bool CheckPlayerPos();

	bool Idle();

	bool Walking();

	bool Move();

	bool Chase();

	bool Attack();

	bool Die();

	//hit enemy
	bool Movebyhit();

private:
	SoldierType soldier_type;
	uint id = 0;

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