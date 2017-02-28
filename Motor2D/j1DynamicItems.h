#pragma once
#ifndef __DYNAMICITEMS_H_
#define __DYNAMICITEMS_H_

#include "j1SceneElements.h"


class Text;

class DynamicItems : public j1SceneElement
{
public:
	DynamicItems(iPoint position);

	// Destructor
	~DynamicItems();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update();

	void Draw();
	// Called before all Updates
	//bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//bool Save();


public:
	SDL_Texture* texture;


};





#endif //__DYNAMICITEMS_H_