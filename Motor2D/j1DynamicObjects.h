#pragma once
#ifndef __DYNAMICOBJECTS_H_
#define __DYNAMICOBJECTS_H_

#include "SceneElements.h"

enum DynObjectState { D_IDLE = 0, D_DYING, D_PICKED, D_AIR, D_IMPACTING };

class DynamicObjects : public SceneElement
{
public:
	DynamicObjects();

	// Destructor
	~DynamicObjects();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint, iPoint pos, bool isSign = false);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	void Draw();

	// Called before quitting
	bool CleanUp();

	//bool Save();

	void ModifyTileCost(int cost);

	// OBJECTS THAT CAN BE THROWED -------
	bool Follow(SceneElement* entity);
	void Throw(Direction dir);
	void KeepGoing(float dt);
	DynObjectState IsImpact(int actual_floor);
	int DropItem();
	//-------------------------------------

	DynObjectState GetState() const;
	void SetState(DynObjectState state);
	void SetAnimState(DynObjectState state);
	std::string GetDialog();


public:
	Collider* collision = nullptr;
	SDL_Rect rect;
	bool pickable = false;
	bool start_impact = false;


private:
	DynObjectState state = D_IDLE;
	DynObjectState anim_state = D_IDLE;
	SceneElement* to_follow = nullptr;
	float speed = 0;
	j1Timer timer;
	float lifetime = 0;
	std::string dialog;

	//Animations
	Animation destroy_animation;
	SDL_Texture* tex = nullptr;
	SDL_Rect anim_rect;
	iPoint pivot;
};





#endif //__DYNAMICOBJECTS_H_