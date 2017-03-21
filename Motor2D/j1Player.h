#pragma once
#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "SceneElements.h"
#include"j1Creature.h"
#include "j1InputManager.h"

class Creature;
class InputListener;

enum DIRECTION
{
	D_UP,
	D_DOWN,
	D_RIGHT,
	D_LEFT
};

enum PLAYER_STATE
{
	P_IDLE,
	P_WALKING,
	P_ATTACKING,
	P_DYING
};

class Text;

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
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Save();

	void OnCollision(Collider*, Collider*);

	bool Camera_inside();

	bool Idle();

	bool Walking();

	bool Move();

	bool CheckOrientation();

	void OnInputCallback(INPUTEVENT, EVENTSTATE);

public:
	SDL_Texture* player_texture;
	SDL_Texture* maptex;
	SDL_Texture* hit_tex;
	SDL_Rect Rect_player;
	std::string tex_player_file_name;
	std::string texmapfile_name;
	std::string file_hit;

	uint gems, bombs, arrows, charge;

private:
	Text* hp_text;
	Text* attack_text;

	std::list<Item*> bag;

	Collider* collision_player;

	//TEST
	std::string stats_temp;

	bool Camera_follow_player;
	bool changeResolution;
	DIRECTION current_direction = D_DOWN;
	PLAYER_STATE curr_state = P_IDLE;

};



#endif