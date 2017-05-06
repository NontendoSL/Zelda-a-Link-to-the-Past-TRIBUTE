#pragma once
#ifndef __POKEMONCOMBAT_H_
#define __POKEMONCOMBAT_H_

#include "NPC.h"
#include "j1Window.h"
#include "j1Collision.h"

enum PokemonCombatState { PC_IDLE = 0, PC_WALKING, PC_ATTACKING, PC_SPECIAL, PC_HIT, PC_DYING, PC_STATIC, PC_HIT, PC_CHASING };
enum PokemonParticles { BUBBLE, LEAF };

class PokemonCombat : public NPC
{
public:
	PokemonCombat()
	{}

	~PokemonCombat()
	{}

	iPoint Getposition() const
	{
		return position;
	}

	PokemonCombatState GetState() const
	{
		return state;
	}
	void SetState(PokemonCombatState s_state)
	{
		state = s_state;
	}
	void SetAnimState(PokemonCombatState a_state)
	{
		anim_state = a_state;
	}

public:

	bool active = false;
	bool dead = false;
	uint cooldown = 0;
	int canmove = 0;

	//target
	PokemonCombat* target = nullptr;

	//special attack
	iPoint range; //.X shows max range, .Y shows distance moved
	iPoint sp_start;
	bool sp_attacking = false;
	int sp_direction = 0;
	int sp_speed = 0;
	uint sp_damage = 0;
	Collider* sp_attack = nullptr;
	Collider* collision_attack = nullptr;

protected:
	//Item* drop;
	Item* item_inside = nullptr;

	PokemonCombatState state = PC_IDLE;
	PokemonCombatState anim_state = PC_IDLE;

	Animation* current_animation = nullptr;

	bool attacker = false;
	bool getdamage = false;

	j1Timer time_wait;

};

#endif //__POKEMONCOMBAT_H_
