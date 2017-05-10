#pragma once
#ifndef __DUSCLOPS_H_
#define __DUSCLOPS_H_

#include "PokemonCombat.h"

#define SPEED 2
#define MAX_SIZE 1000

class Dusclops : public PokemonCombat
{
public:
	Dusclops();

	~Dusclops();

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

	void IncrementSpecial();

	bool Chasing(float dt);

	void OnCollision(Collider*, Collider*);

	bool Movebyhit();

	int CheckPlayerPos();

	bool use_special = false;

private:

	//Timer
	int timetoplay = 0;
	int dis_moved = 0;
	int distance = 0;
	bool reset_run = false;
	bool reset_distance = false;
	int timetorun = 0;

	SDL_Texture* texture_special = nullptr;
	SDL_Rect rect_special;
	iPoint pos_special;
	bool stop_anim_special = false;
	int angle_special = 0;
	//bool drawThrowSP = false; **Only the special attack is launch.**

};

#endif //__DUSCLOPS_H_
