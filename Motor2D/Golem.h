#pragma once
#ifndef __GOLEM_H_
#define __GOLEM_H_

#include "Pokemon.h"

class Golem : public Pokemon
{
public:
	Golem();

	~Golem();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint);

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

	void AddItem(Item*);

	void Drop_item();

	bool CheckPlayerPos();

	bool Idle();

	bool Walking();

	bool Move();

	bool Chase();

	bool Attack();

	bool Death();

	bool Hit();

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

	int timetoplay;
	int timetorun;
	bool reset_run;
	int distance;
	int dis_moved;
	bool reset_distance;
	bool mode_stone;

	j1Timer hurt_timer;
	j1Timer change_dir;
};

#endif //__GOLEM_H_