#pragma once
#ifndef __COMBATMANAGER_H__
#define __COMBATMANAGER_H__

#include "j1Module.h"
#include <list>
#include "SceneElements.h"
#include "PugiXml\src\pugixml.hpp"

class NPC;
class PokemonCombat;
class PokeTrainer;
// ---------------------------------------------------
class CombatManager : public j1Module
{
public:

	CombatManager();

	~CombatManager();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void CreateTargets();

	//Modify to active pokemon (only pokemons Link)
	void PrepareToCombat(PokemonCombat* pokemon);

	//Create Functions 
	PokemonCombat* CreatePokemon(pugi::xml_node&, uint id);
	PokeTrainer* CreateTrainer(pugi::xml_node&, uint id);
	// delete elements
	bool DeleteElements_combat();
	//Change Pokemon
	PokemonCombat* change_pokemon();

	pugi::xml_node conf;
	pugi::xml_document XML;

	PokemonCombat* pokemon_active_link = nullptr;
	PokemonCombat* pokemon_active_trainer = nullptr;

private:
	std::list<SceneElement*> elementcombat;
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	int pokemon_order = 0;
};

#endif // __COMBATMANAGER_H__