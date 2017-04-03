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
class Hookshot;
class Weapon;
class Pokemon;
class BombContainer;

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

	bool Update(float dt); //TODO LOW -> Change bool to void

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// delete elements witout player
	bool DelteElements();

	//Create Functions 
	Soldier* CreateSoldier(uint id, pugi::xml_node& );
	Item* CreateItem(uint id, iPoint position);
	Hookshot* CreateHookshot();
	DynamicObjects* CreateDynObject(iPoint pos, uint id, uint id_map);
	Player* CreatePlayer();
	BombContainer* CreateBombContainer();
	Pokemon* CreatePokemon(pugi::xml_node&, uint id, iPoint pos = (iPoint(0,0)));

	//Delete Functions
	bool DeleteEnemy(Soldier* enemy);
	bool DeleteDynObject(DynamicObjects* dynobject);
	bool DeleteItem(Item* enemy);
	bool DeletePokemon(Pokemon* enemy);

	pugi::xml_node conf;
	pugi::xml_document XML;

private:
	std::list<SceneElement*> elementscene;
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const; //TODO LOW ->We hace this function in App but was with private
	//Delete
	
};

#endif // __j1ENTITYELEMENTSCENE_H__