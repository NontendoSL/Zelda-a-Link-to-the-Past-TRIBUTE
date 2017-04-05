#ifndef __J1CREATURE_H__
#define __J1CREATURE_H__

#include "j1Module.h"
#include "SceneElements.h"
#include "SDL/include/SDL_rect.h"
#include "j1Map.h"
#include "time.h"

enum CreatureType { PLAYER, POKEMON, GANON, SOLDIER, BCTROOPER, POKETRAINER };

// ---------------------------------------------------
class Creature : public SceneElement
{
public:

	Creature() {}
	~Creature() {}

	virtual bool CheckOrientation()
	{
		return true;
	}

	virtual bool Move()
	{
		return true;
	}

	virtual bool Move(int x, int y);

	virtual bool Die()
	{
		return true;
	}

	virtual bool GoTo(iPoint destination, int speed);
	virtual bool Orientate();
	virtual bool OrientateTo(iPoint pos);

public:
	CreatureType c_type;
	int hp;
	bool walking = false;
	int attack;
	float chase_speed;

	//COMBAT STATS
	j1Timer attack_time;
	int attack_speed = 0;
	int radar = 0;
	iPoint target_pos = { 0, 0 };
	std::vector<iPoint> path;

	//COLLISIONS
	Collider* collision_feet = nullptr;
	j1Timer orient_time;


};

#endif // __J1CREATURE_H__