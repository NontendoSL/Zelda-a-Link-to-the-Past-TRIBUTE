#pragma once
#ifndef __COMBATMANAGER_H__
#define __COMBATMANAGER_H__

#include "j1Module.h"
#include <list>
#include "SceneElements.h"
#include "PugiXml\src\pugixml.hpp"

class Player;
class NPC;
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

	bool Update(float dt); //TODO LOW -> Change bool to void

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// delete elements witout player
	bool DelteElements();

	//Create Functions 

	//Delete Functions

	pugi::xml_node conf;
	pugi::xml_document XML;

private:
	std::list<SceneElement*> elementscene;
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const; //TODO LOW ->We hace this function in App but was with private
																	  //Delete

};

#endif // __COMBATMANAGER_H__