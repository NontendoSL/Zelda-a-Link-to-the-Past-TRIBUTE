#pragma once
#ifndef __NPC_H_
#define __NPC_H_

#include "SceneElements.h"
#include "j1Creature.h"

enum StatusAction {GUARD, HUNT};

class NPC : public Creature
{
public:
	NPC() : Creature()
	{}

	~NPC()
	{}

	virtual void Dead()
	{

	}

public:
	bool indestructible;
	bool movable;
	bool canattack;

	int npc_id;
	uint marge;
	StatusAction status_action;
	Collider* collision_enemy = nullptr;


};

#endif //__NPC_H_