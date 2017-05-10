#ifndef __J1CREATURE_H__
#define __J1CREATURE_H__

#include "j1Module.h"
#include "SceneElements.h"
#include "SDL/include/SDL_rect.h"
#include "j1Map.h"
#include "time.h"

// ---------------------------------------------------
class Creature : public SceneElement
{
public:

	Creature() {}
	~Creature() {}

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
	int hp = 0;
	bool walking = false;
	int attack = 0;
	float chase_speed;

	//COMBAT STATS
	int radar = 0;
	iPoint target_pos = { 0, 0 };

	//COLLISIONS
	Collider* collision_feet = nullptr;
	j1Timer orient_time;

	//
	Direction dir_hit;
	iPoint prev_position;
	j1Timer knockback_time;

protected:
	uint speed;
};

#endif // __J1CREATURE_H__