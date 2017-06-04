#pragma once
#ifndef __WEAPONS_H_
#define __WEAPONS_H_

#include "SceneElements.h"
enum HookState { TARGET, OBSTACLE, MISS};
enum BombStep { PLANTED, EXPLOSION };
enum ArrowStep { AIR, WALL_IMPACT, ENEMY_IMPACT, DIE};

enum WeaponType { W_BOW, W_HOOKSHOT, W_BOMB };
enum WeaponState { W_IDLE = 0, W_DYING };


class BombContainer;
class Bow;
class P_Follow;

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
	int speed = 0; //TODO JORDI -> combine with dt

private:
	HookState hook_state = MISS;
};

class Arrow
{
public:
	Arrow(iPoint position, Direction dir, Bow* container, float speed);
	~Arrow();

	void Update(float dt);
	void Draw();

	void KeepGoing(float dt);
	ArrowStep IsImpact(int actual_floor);
	void Die();

public:
	int offset_x = 0;
	int offset_y = 0;
	iPoint position;
	Direction direction = NO_DIRECTION;
	float arrow_speed = 0;
	float lifetime = 0;
	bool start_impact = true;
	j1Timer timer;
	j1Timer impact_time;
	Animation* current = nullptr;
	Bow* container = nullptr;
	ArrowStep step;
	Collider* collision = nullptr;
	P_Follow* particles = nullptr;
};

class Bow : public Weapon
{
public:
	Bow(bool equipable);
	~Bow();

	bool Start();
	bool Update(float dt);
	void Draw();
	void CleanContainer();
	void DestroyArrows();

	float SetSpeed(float charge);
	void Shoot(iPoint pos, Direction dir, float speed);

	//void OnCollision(Collider* c1, Collider* c2);

private:
	std::list<Arrow*> arrows;
	float arrow_speed = 0;
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
	j1Timer timer;
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