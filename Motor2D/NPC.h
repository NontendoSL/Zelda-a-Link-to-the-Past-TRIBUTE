#pragma once
#ifndef __NPC_H_
#define __NPC_H_

#include "SceneElements.h"
#include "j1Creature.h"

class NPC : public Creature
{
public:
	NPC() : Creature()
	{}

	~NPC()
	{}

public:
	bool destructible = false;
	bool movable = false;
	bool canattack = false;

	int npc_id = 0;
	uint marge;


};

#endif //__NPC_H_