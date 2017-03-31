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

	bool Attack();

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

public:
	SDL_Texture* player_texture = nullptr;
	SDL_Texture* maptex = nullptr;
	SDL_Texture* hit_tex = nullptr;
	SDL_Rect Rect_player;
	std::string tex_player_file_name;
	std::string texmapfile_name;
	std::string file_hit;

	//ITEMS ----------
	uint gems = 0;
	uint bombs = 0;
	uint arrows = 0;
	uint charge = 0;

	Hookshot* hook = nullptr;
	Weapon* equiped_item = nullptr;

	//Pokemons has Link
	std::list<Pokemon*> pokedex;

private:
	Text* hp_text = nullptr;
	Text* attack_text = nullptr;

	std::list<Item*> bag;

	Collider* collision_player;
	Collider* collision_attack;

	bool attacker;
	int attack_time;

	//Timer
	int timetoplay;

	//TEST
	std::string stats_temp;

	bool Camera_follow_player = true;
	bool changeResolution = false;

	Animation* current_animation = nullptr;
};



#endif