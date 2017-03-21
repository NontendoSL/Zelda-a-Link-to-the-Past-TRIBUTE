#ifndef __J1CREATURE_H__
#define __J1CREATURE_H__

#include "j1Module.h"
#include "SceneElements.h"
#include "SDL/include/SDL_rect.h"

enum CreatureType { PLAYER, POKEMON, GANON, SOLDIER, BCTROOPER, POKETRAINER };
//enum ActionState { IDLE, WALKING, ATTACKING, DYING };
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

	virtual bool Die()
	{
		return true;
	}


public:
	Direction dir;
	CreatureType c_type;
	//ActionState state;
	uint hp;
	bool walking = false;
	int attack;

};

#endif // __J1CREATURE_H__