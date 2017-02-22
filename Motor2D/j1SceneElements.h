#pragma once
#pragma once
#ifndef __j1SCENEELEMENT_H__
#define __j1SCENEELEMENT_H__

#include "j1Module.h"
#include <list>
#include "p2Point.h"
#include "j1AnimationManager.h"
#include "j1Textures.h"

enum ElementType{PLAYER,ENEMY,ITEM};
// ---------------------------------------------------
class j1SceneElement
{
public:

	j1SceneElement(iPoint position): position(position)
	{}

	~j1SceneElement()
	{}

	virtual bool Awake(pugi::xml_node&) 
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

	virtual bool Save() { return false; };

public:
	iPoint position;
	AnimationState state;
	Direction dir;
	ElementType type;

	std::string	name;
	//SDL_Texture* texture;

};

#endif // __j1SCENEELEMENT_H__