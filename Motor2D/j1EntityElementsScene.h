#pragma once
#ifndef __j1ENTITYELEMENTSCENE_H__
#define __j1ENTITYELEMENTSCENE_H__

#include "j1Module.h"
#include <list>
#include "SceneElements.h"
#include "PugiXml\src\pugixml.hpp"

class Soldier;
class Item;
class Player;
class DynamicObjects;
class NPC;
class Creature;
class Hookshot;
class Bow;
class Weapon;
class Pokemon;
class BombContainer;
class PokeTrainer;
class BCTrooper;
class Villager;
class GreenMinion;
class RedMinion;
class Ganon;
class FireBat;

// ---------------------------------------------------
class j1EntityElementScene : public j1Module
{
public:

	j1EntityElementScene();

	~j1EntityElementScene();

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

	bool DelteWeapons();
	// delete elements witout player
	bool DelteElements();

	//Create Functions 
	void CreateSoldier(uint id, pugi::xml_node& );
	void CreateItem(uint id, iPoint position);
	Hookshot* CreateHookshot();
	Bow* CreateBow();
	void DeleteArrows();
	void CreateDynObject(iPoint pos, uint id, uint id_map, bool isSign = false, pugi::xml_node& conf = pugi::xml_node(NULL));
	Player* CreatePlayer();
	BombContainer* CreateBombContainer();
	void CreatePokemon(pugi::xml_node&, uint id, iPoint pos = (iPoint(0,0)));
	void CreateBCTrooper(pugi::xml_node&);
	void CreateGanon(pugi::xml_node&);
	void CreateVillager(pugi::xml_node&);
	void CreateGMinion(iPoint pos);
	void CreateRMinion(iPoint pos);
	void CreateFireBat();

	//Delete Functions
	bool DeleteEnemy(Soldier* enemy);
	bool DeleteDynObject(DynamicObjects* dynobject);
	bool DeleteItem(Item* enemy);
	bool DeletePokemon(Pokemon* enemy);
	bool DeleteBCTrooper(BCTrooper* bctrooper);
	bool DeleteElement(std::string name);
	bool DeletePlayer(Player* player);
	bool DeleteVilager(Villager* Vilager);
	bool DeleteCreature(Creature* creature);

	// TODO HIGH -> CREATE DELETE FUNCTIONS

	void SwapObject(SceneElement* obj);
	void SwapGanon();
	void SwapPlayer();

	pugi::xml_node conf;
	pugi::xml_document XML;

	//texture dynobjects
	SDL_Texture* texture_dynobjects = nullptr;
	std::string file_tex_dynobjects;
	//texture Brendan
	SDL_Texture* texture_trainer = nullptr;
	std::string file_tex_trainer;

	//dynspecial texture
	SDL_Texture* text_vase_bush = nullptr;
	//wepaon Hookshot chain
	SDL_Texture* hookshot_chain = nullptr;

	Ganon* ganon = nullptr;

private:
	std::list<SceneElement*> elementscene;
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	BCTrooper* bct = nullptr;
	
};

#endif // __j1ENTITYELEMENTSCENE_H__