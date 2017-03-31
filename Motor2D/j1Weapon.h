#pragma once
#ifndef __WEAPONS_H_
#define __WEAPONS_H_

#include "SceneElements.h"
enum HookState {TARGET, OBSTACLE, MISS};

class Weapon : public SceneElement
{
public:
	Weapon();

	~Weapon();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint id);

	// Called before the first frame
	//bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	//bool Update();

	// Called before all Updates
	//bool PostUpdate();

	//void Draw();

	// Called before quitting
	//bool CleanUp();

	//bool Save();

public:
	//bool picked = false;
	bool equiped = false;
	bool equipable = false;

	Collider* collision = nullptr;
};


class Hookshot :public Weapon
{
public:
	Hookshot(bool equipable);
	~Hookshot();

	bool Start();

	bool Update();

	void Draw();

	void SetRange(uint charge);

	//Check the tile at its position each X Updates
	HookState ReachObjective();
	HookState GetState();

	void Reset();

	uint range = 0;
	uint actual_range_pos = 0; // TODO MED-> set to private and create functions to get the values
	bool target_reached = false;
	bool in_use = false;

private:
	HookState hook_state = MISS;
};

class Bow : public Weapon
{
	Bow(bool equipable);
	~Bow();

	bool Start();

	bool Update();

	void SetSpeed(uint charge);

	//void OnCollision(Collider* c1, Collider* c2);

private:
	uint arrow_speed = 0;
	std::vector<Item*> throwed_arrows;
};

#endif