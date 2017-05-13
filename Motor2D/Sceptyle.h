#pragma once
#ifndef __SCEPTYLE_H_
#define __SCEPTYLE_H_

#include "PokemonCombat.h"

class Sceptyle : public PokemonCombat
{
public:
	Sceptyle();

	~Sceptyle();

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

	//Special Attack
	void Special_Attack();
	void ThrowSP();

	bool Idle();

	bool Walking(float dt);

	bool Move(float dt);

	bool Attack();

	void Stunned();

	bool Movebyhit(int speed);

private:

	//Timer
	int timetoplay = 0;
	int dis_moved = 0;
	int distance = 0;
	bool reset_run = false;
	bool reset_distance = false;
	int timetorun = 0;
	bool drawThrowSP = false;

	//SP
	 SDL_Texture* sp_leaf = nullptr;
	 SDL_Rect sp_rect = { 47, 265, 13, 13 };
	std::vector<iPoint>leafs_pos;
	uint rotation = 0;
	Collider* top_leaf_sp = nullptr;
	Collider* bot_leaf_sp = nullptr;
	bool leaving = true;
};

#endif //__SCEPTYLE_H_