#pragma once
#ifndef __DYNAMICOBJECTS_H_
#define __DYNAMICOBJECTS_H_

#include "SceneElements.h"


class Text;

class DynamicObjects : public SceneElement
{
public:
	DynamicObjects();

	// Destructor
	~DynamicObjects();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint, iPoint pos);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	void Draw();
	// Called before all Updates
	//bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//bool Save();


public:

	Collider* collision = nullptr;
	SDL_Rect rect;
	int item_id = 0;
};





#endif //__DYNAMICOBJECTS_H_