#pragma once
#ifndef __GANON_H_
#define __GANON_H_

#include "NPC.h"

#define NUM_POINTS_CIRCLE 90
#define MULTI_P 4

enum GanonPhase { SLEEP = 0, INITIAL, INVINCIBLE, RAGE, DEATH };
enum GanonState { G_WALKING = 0, G_HIT, G_MELEE, G_SPECIAL_1/*Jump&Fire*/, G_SPECIAL_2/*FireBats*/, G_SHIELD, G_DYING, G_ATTACKING, G_IDLE};
enum SpawnType { GREEN, RED };

class Ganon : public NPC
{
public:
	Ganon();

	~Ganon();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// PHASE UPDATES MACHINE -------------
	bool InitialUpdate(float dt);
	bool InvincibleUpdate(float dt);
	bool RageUpdate(float dt);
	bool DeathUpdate(float dt);
	// ----------------------------------

	// STATE MACHINE ----------------
	void Idle();
	void Walk(float dt);
	bool Move(float dt);
	void Reorientate();
	void MeleeAttack();
	void SpecialAttack();
	void FireBats();
	void FireCircle();
	void Hit();
	void Spawn();
	// -----------------------------

	// Called before quitting
	bool CleanUp();

	void OnCollision(Collider*, Collider*);

	GanonState GetState();
	void SetState(GanonState state);
	void SetAnimState(GanonState state);

private:
	GanonState state = G_WALKING;
	GanonState anim_state = G_WALKING;
	GanonPhase phase = SLEEP;
	Animation* current_animation = nullptr;
	int item_id = -1;

	//HIT TIMERS/VARIABLES ---
	j1Timer HitTime;
	// -------------------------

	//ATTACK VARIABLES ---
	bool StartAttack = false;
	Collider* collision_attack = nullptr;
	// ----------------------

	//MOVEMENT VARIABLES ---
	int canmove = 0;
	// -------------------

	//2ND PHASE VARIABLES ---
	j1Timer spawn_timer;
	int spawn_rate = 2;
	int minions_killed = 0;
	int minions_spawned = 0;
	// -----------------------

	//3D PHASE VARIABLES -----
	int radius = 10;
	j1Timer firebat_spawn;
	float firebat_rate = 0.4;
	std::vector<iPoint> points;
	GanonState special_attack = G_SPECIAL_1;
	// -----------------------

};

#endif //__GANON_H_