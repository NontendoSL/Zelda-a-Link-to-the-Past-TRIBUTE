#pragma once
#ifndef __j1ENTITYELEMENTSCENE_H__
#define __j1ENTITYELEMENTSCENE_H__

#include "j1Module.h"
#include <list>
#include "j1SceneElements.h"

class j1Enemy;
class j1Item;
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

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	//TODO LOW
	j1Enemy* CreateEnemy(iPoint position, int hp, int attack, float speed);
	j1Item* CreateItem(iPoint position);
	Player* CreatePlayer(iPoint position);

private:
	std::list<j1SceneElement*> elementscene;

	//Delete
	
};

#endif // __j1ENTITYELEMENTSCENE_H__