#pragma once
#ifndef __BLAZIKEN_H_
#define __BLAZIKEN_H_

#include "Pokemon.h"


class Blaziken : public Pokemon
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
	bool Update();

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// Called before quitting
	bool CleanUp();

	bool Idle();

	bool Walking();

	bool Move();

	bool Attack();

	bool Idle_IA();

	bool Walking_IA();

	bool Move_IA();

	bool Attack_IA();

	bool CheckOrientation();

	void OnCollision(Collider*, Collider*);

	bool Movebyhit();

private:

	//Timer
	int timetoplay = 0;
	int dis_moved = 0;
	int distance = 0;
	bool reset_run = false;
	bool reset_distance = false;
	int timetorun = 0;


};

#endif //__BLAZIKEN_H_