#pragma once
#ifndef __POKETRAINER_H_
#define __POKETRAINER_H_

#include "NPC.h"
#include "Pokemon.h"

enum TrainerState { T_IDLE = 0, T_TALKING };

class PokeTrainer : public NPC
{
public:
	PokeTrainer();

	~PokeTrainer();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// Called before quitting
	bool CleanUp();

	Pokemon* GetPokemon(uint position);

	std::list<Pokemon*> pokedex;

private:
	TrainerState state = T_IDLE;
	TrainerState anim_state = T_IDLE;
	//Item* drop;
	bool stunned = false;

	//SDL_Texture* texture = nullptr;
	//std::string file_name;
	bool active = false;

};

#endif //__POKETRAINER_H_

