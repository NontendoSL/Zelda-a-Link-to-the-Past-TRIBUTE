#pragma once
#ifndef __POKEMONCOMBAT_H_
#define __POKEMONCOMBAT_H_

#include "NPC.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "CombatManager.h"

enum PokemonCombatState { PC_IDLE = 0, PC_WALKING, PC_ATTACKING, PC_SPECIAL, PC_HIT, PC_DYING, PC_STATIC, PC_STUNNED, PC_CHASING };
enum PokemonParticles { BUBBLE, LEAF };

class PokemonCombat : public NPC
{
public:
	PokemonCombat()
	{}

	virtual ~PokemonCombat()
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

	void ModifyStats(const Item_pokeCombat* items)
	{
		if (items->def_protein)
		{
			defense += 1;
		}
		if (items->hp_up)
		{
			hp += 50;
		}
		if (items->x_attack)
		{
			attack += 4;
			sp_damage += 8;
		}
	}

	bool OrientatePokeLink()
	{
		iPoint pokemon = App->combatmanager->pokemon_active_link->position;

		Direction prev_dir = direction;
		if (abs(pokemon.x - position.x) < abs(pokemon.y - position.y))
		{
			if (pokemon.y > position.y)
			{
				direction = DOWN;
			}

			else
			{
				direction = UP;
			}
		}

		else
		{
			if (pokemon.x < position.x)
			{
				direction = LEFT;
			}

			else
			{
				direction = RIGHT;
			}
		}

		if (prev_dir == direction)
		{
			return false;
		}

		else
		{
			return true;
		}
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
	//special attack stats
	iPoint sp_start;
	bool sp_attacking = false;
	int sp_direction = 0;
	int sp_speed = 0;
	uint sp_damage = 0;

	uint defense = 0;

	Collider* sp_attack = nullptr;
	Collider* collision_attack = nullptr;
	j1Timer time_stunned;

protected:

	PokemonCombatState state = PC_IDLE;
	PokemonCombatState anim_state = PC_IDLE;

	Animation* current_animation = nullptr;

	bool attacker = false;
	bool getdamage = false;

};

#endif //__POKEMONCOMBAT_H_
