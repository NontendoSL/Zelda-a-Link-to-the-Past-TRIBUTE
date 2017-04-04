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
	bool destructible;
	bool movable;
	bool canattack;

	int npc_id;
	uint marge;


};

#endif //__NPC_H_