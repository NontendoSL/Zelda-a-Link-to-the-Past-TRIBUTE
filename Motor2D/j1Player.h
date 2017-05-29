#pragma once
#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "SceneElements.h"
#include "j1Creature.h"
#include "j1InputManager.h"

enum LinkState { L_IDLE = 0, L_WALKING, L_ATTACKING, L_WALKSHIELD, L_BOW, L_PICKING, L_FALL, L_HIT, L_HOOKTHROWN, L_PUSH, L_INTERACTING, L_DYING, };

class Creature;
class InputListener;
class Animation;
class Text;
class PokemonCombat;
class Hookshot;
class Weapon;
class Bow;
class BombContainer;
class Dialogue;
class Image;
class DynamicObjects;

struct CheckPoint
{
	//MAP
	iPoint pos = { 0, 0 };
	int map_id = 0;

	//UI
	std::string world;

	//ZELDA WORLD ---------------
	//STATS
	iPoint hp_hearts{ 6, 6 };

	//RESOURCES
	uint rupees = 0;
	uint bombs = 0;
	uint arrows = 0;

	//WEAPONS
	bool bow_picked = false;
	bool bombcontainer_picked = false;
	bool hookshot_picked = false;
	bool sword_picked = false;
	//-----------------------------

	//POKEMON WORLD --------
	uint cash = 0;
	int blaz_hp = 0;
	int blaz_atk = 0;
	int blaz_def = 0;
	int scept_hp = 0;
	int scept_atk = 0;
	int scept_def = 0;
	int swamp_hp = 0;
	int swamp_atk = 0;
	int swamp_def = 0;
	//----------------------
};

class Player : public Creature
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

	bool SaveData(pugi::xml_node&);

	bool Save();

	void OnCollision(Collider*, Collider*);

	bool Camera_inside(Direction direction);

	bool Camera_inside(iPoint pos);

	//STATE MACHINE -------
	bool Idle();
	bool Walking(float dt);
	bool Move(float dt);
	bool Hit();
	bool Attack();
	bool Interact();
	//----------------------
	void ThrowObject();

	void GetfloorLvl(iPoint pos);
	int GetFloor() const;

	bool Equip(Weapon* to_equip);
	bool Unequip();

	//HOOKSHOT FUNCTIONALITY -----------
	void ThrowHookshot(uint charge);
	bool Hooking();
	void KeepGoing();
	void PickUpHook();
	void MoveTo(const iPoint& pos);
	//----------------------------------

	int GetnuminputUse();

	void AddHeartContainer();
	void ShowHearts();
	void GetDamage();
	void SetKnockbackDir();

	LinkState GetState() const;
	void SetState(LinkState state);
	void SetAnimState(LinkState state);

	//Comprovate camera in map
	bool CameraisIn();

	//LOAD/SAVE FUNCTION ----
	void SaveCheckPoint(int map_id);
	void LoadStats();
	//-----------------------

public:
	//--- POKEMON TRAINER CARD ---
	j1Timer game_timer;
	Text* time = nullptr;
	int score;

	//ITEMS ----------
	uint gems = 0;
	uint pokecash = 0;
	uint bombs = 0;
	uint arrows = 0;
	uint charge = 0;
	Dialogue* dialog = nullptr;
	Hookshot* hook = nullptr;
	BombContainer* bombmanager = nullptr;
	Bow* bow = nullptr;

	Weapon* equiped_item = nullptr;
	DynamicObjects* picked_object = nullptr;

	//Pokemons has Link
	std::list<PokemonCombat*> pokedex;
	bool state_complet = false;
	bool lose_combat = false;

	bool camera_follow = false;

	//Gameover TODO MARC
	Image* gameover = nullptr;
	Image* winover = nullptr;

	//cooldown for soldier chase
	j1Timer invincible_timer;
	j1Timer hurt_timer;

	//Current release test
	bool sword_equiped = false;

	//Weapons test
	bool setWeapons = false;

	CheckPoint checkpoint;

private:

	iPoint hp_hearts;//.X shows MAX HEARTS and .Y ACTUAL LIFE (1 heart on hud == 2 hp_hearts)
	int actual_floor = 0;
	int before_map = 0;

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

	//T
	std::string stats_temp;

	//bool changeResolution = false;

	Animation* current_animation = nullptr;
};



#endif