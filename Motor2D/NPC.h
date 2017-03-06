#pragma once
#ifndef __SOLDIER_H_
#define __SOLDIER_H_

#include "SceneElements.h"

class NPC : public SceneElement
{
public:
	NPC(iPoint position) : SceneElement(position)
	{}

	~NPC()
	{}

	virtual void Dead()
	{

	}

public:
	uint hp;
	uint attack_dmg;
	uint speed;
	bool indestructive;
	bool movable;
	bool canattack;

};

#endif //__SOLDIER_H_