#pragma once
#ifndef __j1ENTITYELEMENTSCENE_H__
#define __j1ENTITYELEMENTSCENE_H__

#include "j1Module.h"
#include <list>
#include "j1SceneElements.h"
#include "PugiXml\src\pugixml.hpp"

class Enemy;
class Item;
class Player;
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

	bool Update(float dt); //TODO LOW -> Change bool to void

	// Called before quitting
	bool CleanUp();

	//TODO LOW
	Enemy* CreateEnemy(iPoint position, uint id);
	bool DeleteEnemy(Enemy* enemy);
	Item* CreateItem(iPoint position, uint id);
	Player* CreatePlayer(iPoint position);

	pugi::xml_node conf;
	pugi::xml_document XML;
private:
	std::list<j1SceneElement*> elementscene;
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	//Delete
	
};

#endif // __j1ENTITYELEMENTSCENE_H__