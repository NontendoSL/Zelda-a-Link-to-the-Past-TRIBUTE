#pragma once
#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "SceneElements.h"
#include "j1Creature.h"
#include "j1InputManager.h"

enum LinkState { L_IDLE = 0, L_WALKING, L_ATTACKING, L_DYING, L_HOOKTHROWN, L_HIT, L_INTERACTING };

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
	bool Update(float dt);

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

	//HOOKSHOT FUNCTIONALITY -----------
	void ThrowHookshot(uint charge);
	bool Hooking();
	void KeepGoing();
	void PickUpHook();
	void MoveTo(const iPoint& pos);
	//----------------------------------
	void OnInputCallback(INPUTEVENT, EVENTSTATE);

	int GetnuminputUse();

	void AddHeartContainer();
	void ShowHearts();
	void GetDamage();

	void SetState(LinkState state);
	LinkState GetState() const;
	void SetAnimState(LinkState anim);

	//Comprovate camera in map
	bool CameraisIn();

public:
	//--- POKEMON TRAINER CARD ---
	j1Timer game_timer;
	Text* time = nullptr;
	int score;
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

	//Gameover TODO MARC
	Image* gameover = nullptr;
	Image* winover = nullptr;

	//cooldown for soldier chase
	j1Timer invincible_timer;

private:

	//player stats TODO JORDI - ELLIOT
	iPoint hp_hearts;//.X shows MAX HEARTS and .Y ACTUAL LIFE (1 heart on hud == 2 hp_hearts)
	int actual_floor = 0;

	LinkState state = L_IDLE; //Logical state
	LinkState anim_state = L_IDLE; //Animation state

	//switchmap
	bool canSwitchMap = false;

	//Colliders
	Collider* collision_attack = nullptr;
	Collider* collision_interact = nullptr;

	bool attacker = false;
	bool interaction = false;

	//Timers
	j1Timer timer;
	j1Timer hurt_timer;
	

	//T
	std::string stats_temp;

	//bool changeResolution = false;

	Animation* current_animation = nullptr;
};



#endif