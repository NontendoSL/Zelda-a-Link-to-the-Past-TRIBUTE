#pragma once
#ifndef __WEAPONS_H_
#define __WEAPONS_H_

#include "SceneElements.h"
enum HookState {TARGET, OBSTACLE, MISS};
enum BombStep { PLANTED, EXPLOSION };
enum WeaponType { W_BOW, W_HOOKSHOT, W_BOMB };

enum WeaponState { W_IDLE = 0, W_DYING };


class BombContainer;

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
	bool in_use = false;


	WeaponType Wtype;
	Collider* collision = nullptr;

protected:
	WeaponState state = W_IDLE;
	WeaponState anim_state = W_IDLE;
};


class Hookshot :public Weapon
{
public:
	Hookshot(bool equipable);
	~Hookshot();

	bool Start();

	bool Update(float dt);

	void Draw();

	void SetRange(float charge);

	//Check the tile at its position each X Updates
	HookState ReachObjective(int actual_floor);
	HookState GetState();

	void Reset();

	float range = 0;
	uint actual_range_pos = 0; // TODO MED-> set to private and create functions to get the values
	bool target_reached = false;


public:
	uint speed; //TODO JORDI

private:
	HookState hook_state = MISS;
};

class Bow : public Weapon
{
	Bow(bool equipable);
	~Bow();

	bool Start();

	bool Update(float dt);

	void SetSpeed(uint charge);

	//void OnCollision(Collider* c1, Collider* c2);

public:
	uint speed; //TODO JORDI

private:
	uint arrow_speed = 0;
	std::vector<Item*> throwed_arrows;


};

class Bomb
{
public:
	Bomb(iPoint position, BombContainer* container);
	~Bomb();
public:
	void Update(float dt);
	void Draw();
	void Die();


public:
	iPoint position;
	int radius = 0;
	uint timer;
	Animation* current = nullptr;
	BombContainer* container = nullptr;
	BombStep step;
	Collider* collision = nullptr;
};

class BombContainer :public Weapon
{
public:
	BombContainer();
	~BombContainer();

public:
	void Drop(iPoint position);
	bool Update(float dt);
	void Draw();
	void CleanContainer();


private:
	std::list<Bomb*>bombs;
};
#endif