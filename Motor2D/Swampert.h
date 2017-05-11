#pragma once
#ifndef __SWAMPERT_H_
#define __SWAMPERT_H_

#include "PokemonCombat.h"

class Swampert : public PokemonCombat
{
public:
	Swampert();

	~Swampert();

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

	void OnCollision(Collider*, Collider*);

	bool Idle();

	bool Walking(float dt);

	bool Move(float dt);

	bool Attack();

	bool Movebyhit(int speed);

	void Stunned();

	void Special_Attack();
	void ThrowSP();

private:

	//Timer
	int timetoplay = 0;
	int dis_moved = 0;
	int distance = 0;
	bool reset_run = false;
	bool reset_distance = false;
	int timetorun = 0;
	bool drawThrowSP = false;
};

#endif //__SWAMPERT_H_