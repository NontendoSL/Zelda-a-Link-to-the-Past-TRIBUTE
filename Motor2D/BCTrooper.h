#pragma once
#ifndef __BCTROOPER_H_
#define __BCTROOPER_H_

#include "NPC.h"

#define NUM_POINTS_CIRCLE 90
#define MULTI_P 4

enum BCTrooperState { BC_IDLE = 0, BC_WALKING, BC_HIT, BC_DYING };

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
	bool Update(float dt);

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// Called before quitting
	bool CleanUp();

	// STATE MACHINE ----------------
	void Idle();
	void Walk(float dt);
	void Hit();
	void Death();
	// -----------------------------
	//Change Radius
	bool ChangeRadius(int radius_to_stop, bool increment);

	//Collision----------------------------------
	void OnCollision(Collider*, Collider*);

private:

	BCTrooperState state = BC_IDLE;
	bool stunned = false;
	SDL_Texture* texture = nullptr;
	std::vector<iPoint> points;
	iPoint bole;
	int pos_in_vect = 1;
	int radius = 10;
	j1Timer Change_State;
	bool reset_time = true;
	Collider* collision_maze = nullptr;
};

#endif//__BCTROOPER_H_
