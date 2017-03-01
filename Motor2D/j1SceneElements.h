#pragma once
#pragma once
#ifndef __j1SCENEELEMENT_H__
#define __j1SCENEELEMENT_H__

#include "j1Module.h"
#include <list>
#include "p2Point.h"
#include "j1AnimationManager.h"
#include "j1Textures.h"
#include "SDL/include/SDL_rect.h"

class Item;

enum ElementType{PLAYER,ENEMY,ITEM, DYNITEMS};
// ---------------------------------------------------
class j1SceneElement
{
public:

	j1SceneElement(iPoint position): position(position)
	{}

	~j1SceneElement()
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

public:
	iPoint position;
	AnimationState state;
	Direction dir;
	ElementType type;
	uint speed;

	uint hp;
	int width;
	int height;

	bool canBlit;
	std::string	name;
	//SDL_Texture* texture;

};

#endif // __j1SCENEELEMENT_H__