#pragma once
#ifndef __GEODUDE_H_
#define __GEODUDE_H_

#include "Pokemon.h"

class Geodude : public Pokemon
{
public:
	Geodude();

	~Geodude();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint, iPoint);

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

	bool Walking();

	bool Move();

	bool Attack();

	bool Death();

	bool Movebyhit();

	void OnCollision(Collider*, Collider*);

	Direction dir_hit;
	j1Timer knockback_time;

private:

	//Animation
	AnimationStruct animation;
	SDL_Rect anim_rect;
	iPoint pivot;

	Item* item_inside = nullptr;
	SDL_Texture* texture = nullptr;

	int timetoplay = 0;
	int timetorun = 0;
	bool reset_run = false;
	int distance = 0;
	int dis_moved = 0;
	bool reset_distance = false;

	bool mode_stone = false;


	j1Timer hurt_timer;

	//COMBAT INFO --------
	//Collider* collision_punch = nullptr;
};

#endif //__GEODUDE_H_