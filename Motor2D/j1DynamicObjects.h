#pragma once
#ifndef __DYNAMICOBJECTS_H_
#define __DYNAMICOBJECTS_H_

#include "SceneElements.h"

enum DynObjectState { D_IDLE = 0, D_DYING, D_PICKED };

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

	void Erase(int px, int py);


public:
	Collider* collision = nullptr;
	SDL_Rect rect;
	int item_id = 0;
	bool pickable = false;

private:
	DynObjectState state = D_IDLE;
	DynObjectState anim_state = D_IDLE;
};





#endif //__DYNAMICOBJECTS_H_