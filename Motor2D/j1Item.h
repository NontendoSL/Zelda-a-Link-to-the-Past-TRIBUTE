#pragma once
#ifndef __ITEMS_H_
#define __ITEMS_H_

#include "j1SceneElements.h"


enum itemtype{ZELDA, POKEMON};

class Item : public j1SceneElement
{
public:
	Item(iPoint position);

	~Item();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update();

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// Called before quitting
	bool CleanUp();

	bool Save();

private:
	bool picked=false, equiped, equipable;
	SDL_Texture* texture;
};

#endif