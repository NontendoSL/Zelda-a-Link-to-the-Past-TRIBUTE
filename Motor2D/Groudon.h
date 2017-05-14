#pragma once
#ifndef __GROUDON_H_
#define __GROUDON_H_

#include "PokemonCombat.h"

struct Bole_Fire
{
	iPoint position;
	SDL_Rect rect;
	bool jump_finished = false;
	bool stop = false;
	Collider* collider = nullptr;
	j1Timer jump_timer;
	float jump_time = 0;
	iPoint jump_origin;
	iPoint jump_dest;
	j1Timer time_in_finished;
	bool active = true;
};

class Groudon : public PokemonCombat
{
public:
	Groudon();

	~Groudon();

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

	void CreateBoleFire();

	void FireJump(Bole_Fire*);

	void DoJump(Bole_Fire*);

private:
	std::vector<Bole_Fire*> bole_special;
	SDL_Texture* texture = nullptr;
	//Timer
	int timetoplay = 0;
	int dis_moved = 0;
	int distance = 0;
	bool reset_run = false;
	bool reset_distance = false;
	int timetorun = 0;
	int use_cooldown = 0;
	int max_boles = 0;

};

#endif //__GROUDON_H_

