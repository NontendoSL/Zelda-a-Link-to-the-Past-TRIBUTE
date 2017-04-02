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

	void AddItem(Item*);

	void Drop_item();

	bool Idle();

	bool Walking();

	bool Move();

	bool Attack();

	bool Death();

	bool Movebyhit();

	Direction dir_hit;
	iPoint previus_position;

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
};

#endif //__GEODUDE_H_