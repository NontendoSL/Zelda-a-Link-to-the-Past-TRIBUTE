#pragma once
#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "SceneElements.h"
#include "j1Creature.h"
#include "j1InputManager.h"

class Creature;
class InputListener;
class Animation;
class Text;
class Pokemon;
class Hookshot;
class Weapon;
class BombContainer;
class Dialogue;

class Player : public Creature, public InputListener
{
public:
	Player();

	// Destructor
	~Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update();

	void Draw();

	// Called before all Updates
	//bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Save();

	void OnCollision(Collider*, Collider*);

	bool Camera_inside();

	bool Idle();

	bool Walking();

	bool Move();

	void GetfloorLvl(iPoint pos);

	bool Attack();

	bool Interact();

	bool Equip(Weapon* to_equip);
	bool Unequip();

	//HOOKSHOT FUNCTIONALITY
	void ThrowHookshot(uint charge);
	bool Hooking();
	void KeepGoing();
	void PickUpHook();
	void MoveTo(const iPoint& pos);

	bool CheckOrientation();

	void OnInputCallback(INPUTEVENT, EVENTSTATE);

	int GetnuminputUse();

	void AddHeartContainer();
	void ShowHearts();
	void GetDamage();

public:

	//ITEMS ----------
	uint gems = 0;
	uint bombs = 0;
	uint arrows = 0;
	uint charge = 0;
	Dialogue* dialog = nullptr;
	Hookshot* hook = nullptr;
	BombContainer* bombmanager = nullptr;
	Weapon* equiped_item = nullptr;

	//Pokemons has Link
	std::list<Pokemon*> pokedex;

	bool camera_follow = false;

private:

	//player stats
	iPoint hp_hearts;//.X shows MAX HEARTS and .Y ACTUAL LIFE (1 heart on hud == 2 hp_hearts)
	int actual_floor = 0;
	std::list<Item*> bag;

	//switchmap
	bool canSwitchMap;
	Collider* changeMap;

	//Colliders
	Collider* collision_feet;
	Collider* collision_attack;
	Collider* collision_interact;

	bool attacker = false;
	bool interaction = false;
	bool hurt = false;

	//Timer
	int timetoplay;
	j1Timer timer;

	//TEST
	std::string stats_temp;

	bool changeResolution = false;

	Animation* current_animation = nullptr;
};



#endif