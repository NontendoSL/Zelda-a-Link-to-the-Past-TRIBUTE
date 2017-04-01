#pragma once
#ifndef __SCENEELEMENT_H__
#define __SCENEELEMENT_H__

#include "j1Module.h"
#include <list>
#include "p2Point.h"
#include "j1AnimationManager.h"
#include "j1Textures.h"
#include "j1InputManager.h"
#include "SDL/include/SDL_rect.h"

class Item;

enum ElementType{ CREATURE, ITEM, DYNOBJECT, WEAPON };
enum Direction { UP, DOWN, LEFT, RIGHT };
enum GameState{INGAME, INMENU, TIMETOPLAY};
enum ActionState { IDLE = 0, WALKING, ATTACKING, HOOKTHROWN, HIT, DYING}; //DYING ALWAYS LAST STATE


// ---------------------------------------------------
class SceneElement 
{
public:

	SceneElement()
	{}

	~SceneElement()
	{}

	virtual bool Awake(pugi::xml_node& conf, uint id = 0) 
	{
		return true;
	};

	virtual bool Start() 
	{ 
		return true; 
	};

	virtual bool Update()
	{
		return true;
	};

	virtual void Draw() {};

	virtual void Handle_Input() {};

	virtual bool CleanUp() 
	{
		return true;
	};

	virtual void AddItem(Item* item) {};

	virtual bool Save() { return false; };

	virtual void OnCollision(Collider*, Collider*) {}

	void SetPos(const iPoint& pos);

public:
	iPoint position;
	int offset_x = 0;
	int offset_y = 0;

	Direction direction;
	GameState gamestate;
	ActionState state;
	ElementType type;
	uint speed;


	bool canBlit = false;
	std::string	name;

	uint scale;
	//SDL_Texture* texture;

};

#endif // __SCENEELEMENT_H__