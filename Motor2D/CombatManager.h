#pragma once
#ifndef __COMBATMANAGER_H__
#define __COMBATMANAGER_H__

#include "j1Module.h"
#include <list>
#include "SceneElements.h"
#include "PugiXml\src\pugixml.hpp"

struct Item_pokeCombat
{
	bool hp_up = false;
	bool def_protein = false;
	bool x_attack = false;
};

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
	void PrepareToCombat(PokemonCombat* pokemon, uint idMap, int id_pokemon);

	void ModifyStats(pugi::xml_node&, PokemonCombat* pokemon, Item_pokeCombat bag);

	void CreateDynObject(iPoint pos, uint id, uint id_map);

	int Getsize_elements();

	bool GiveItem(int id_pokemon, const char* item_name);

	//Create Functions 
	PokemonCombat* CreatePokemon(pugi::xml_node&, uint id);
	void CreateTrainer(pugi::xml_node&, uint id);
	// delete elements
	bool DeleteElements_combat();
	//Change Pokemon
	PokemonCombat* change_pokemon();

	pugi::xml_node conf;
	pugi::xml_document XML;

	PokemonCombat* pokemon_active_link = nullptr;
	PokemonCombat* pokemon_active_trainer = nullptr;

	uint id_map_combat = 0;

private:
	std::list<SceneElement*> elementcombat;
	std::vector<Item_pokeCombat> bag_items;
	//PokeTrainer* trainer;
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	int pokemon_order = 0;
};

#endif // __COMBATMANAGER_H__