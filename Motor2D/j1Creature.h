#ifndef __J1CREATURE_H__
#define __J1CREATURE_H__

#include "j1Module.h"
#include "SceneElements.h"
#include "SDL/include/SDL_rect.h"

enum CreatureType { PLAYER, POKEMON, GANON, SOLDIER, BCTROOPER, POKETRAINER };
enum ActionState { IDLE, WALKING, ATTACK, DYING };
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
	CreatureType c_type;
	ActionState state;
	uint hp;
	bool walking = false;
	int attack;

};

#endif // __J1CREATURE_H__