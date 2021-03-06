#pragma once
#ifndef __BLAZIKEN_H_
#define __BLAZIKEN_H_

#include "PokemonCombat.h"


class Blaziken : public PokemonCombat
{
public:
	Blaziken();

	~Blaziken();

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

	void Stunned();

	bool Special_Attack();

	void OnCollision(Collider*, Collider*);

	bool Movebyhit(int speed);

private:

	//Timer
	int timetoplay = 0;
	int dis_moved = 0;
	int distance = 0;
	bool reset_run = false;
	bool reset_distance = false;
	int timetorun = 0;
	j1Timer autoAttack;
	bool dusclops_special = false;


};

#endif //__BLAZIKEN_H_