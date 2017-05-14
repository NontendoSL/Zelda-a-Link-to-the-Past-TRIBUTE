#pragma once
#ifndef __SALAMANCE_H_
#define __SALAMANCE_H_

#include "PokemonCombat.h"

class P_Fire;

class Salamance : public PokemonCombat
{
public:
	Salamance();

	~Salamance();

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

	bool Chasing(float dt);

	void OnCollision(Collider*, Collider*);

	bool Movebyhit(int speed);

	int CheckPlayerPos();

	//void Create

private:

	//Timer
	int timetoplay = 0;
	int dis_moved = 0;
	int distance = 0;
	bool reset_run = false;
	bool reset_distance = false;
	int timetorun = 0;
	int use_cooldown = 0;

	P_Fire* particle_fire = nullptr;

};

#endif //__SALAMANCE_H_

