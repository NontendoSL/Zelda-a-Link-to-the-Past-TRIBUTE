#pragma once
#ifndef __SHIFTRY_H_
#define __SHIFTRY_H_

#include "PokemonCombat.h"

class Shiftry : public PokemonCombat
{
public:
	Shiftry();

	~Shiftry();

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

	bool Idle();

	bool Walking(float dt);

	bool Move(float dt);

	bool Attack();

	void Special_Attack();
	//void ThrowSP(); **Only the special attack is launch.**

	bool Chasing(float dt);

	void OnCollision(Collider*, Collider*);

	bool Movebyhit();

	int CheckPlayerPos();

private:

	//Timer
	int timetoplay = 0;
	int dis_moved = 0;
	int distance = 0;
	bool reset_run = false;
	bool reset_distance = false;
	int timetorun = 0;
	int hp_max = 0;
	//bool drawThrowSP = false; **Only the special attack is launch.**

};

#endif //__SHIFTRY_H_
