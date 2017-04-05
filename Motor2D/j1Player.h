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
class Image;

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

	bool Camera_inside(iPoint pos);

	//STATE MACHINE -------

	bool Idle();
	bool Walking();
	bool Move();
	bool Hit();
	bool Attack();
	bool Interact();

	//----------------------

	void GetfloorLvl(iPoint pos);

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
	Collider* GetCollisionAttack() const;

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

	//Timer
	j1Timer timetoplay;

	//Gameover
	Image* gameover = nullptr;

private:

	//player stats
	iPoint hp_hearts;//.X shows MAX HEARTS and .Y ACTUAL LIFE (1 heart on hud == 2 hp_hearts)
	int actual_floor = 0;
	std::list<Item*> bag;

	//switchmap
	bool canSwitchMap;
	int black;

	//Colliders
	Collider* collision_attack;
	Collider* collision_interact;

	bool attacker = false;
	bool interaction = false;
	bool hurt = false;

	ActionState test_state; //TODO MED -> DELETE THIS

	//Timers
	j1Timer timer;
	j1Timer hurt_timer;
	j1Timer invincible_timer;

	//T
	std::string stats_temp;

	bool changeResolution = false;

	Animation* current_animation = nullptr;
};



#endif