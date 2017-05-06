#pragma once
#ifndef __POKEMON_H_
#define __POKEMON_H_

#include "NPC.h"
#include "j1Window.h"
#include "j1Collision.h"

enum PokemonState { P_IDLE = 0, P_WALKING, P_ATTACKING, P_DYING, P_SPECIAL, P_STATIC, P_HIT, P_CHASING };
enum PokemonParticles { BUBBLE, LEAF};

class Pokemon : public NPC
{
public:
	Pokemon()
	{}

	~Pokemon()
	{}

	iPoint Getposition() const
	{
		return position;
	}
	
	PokemonState GetState() const
	{
		return state;
	}
	void SetState(PokemonState s_state)
	{
		state = s_state;
	}
	void SetAnimState(PokemonState a_state)
	{
		anim_state = a_state;
	}

public:

	bool active = false;
	bool dead = false;
	Collider* collision_attack = nullptr;

protected:
	//Item* drop;
	Item* item_inside= nullptr;
	PokemonState state = P_IDLE;
	PokemonState anim_state = P_IDLE;
	Animation* current_animation = nullptr;

};

#endif //__POKEMON_H_