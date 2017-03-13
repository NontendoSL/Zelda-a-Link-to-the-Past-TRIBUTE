#pragma once
#ifndef __NPC_H_
#define __NPC_H_

#include "SceneElements.h"

class NPC : public SceneElement
{
public:
	NPC() : SceneElement()
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

	int npc_id;
	uint marge;


};

#endif //__NPC_H_