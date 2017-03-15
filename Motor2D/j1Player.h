#pragma once
#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "SceneElements.h"


class Text;

class Player : public SceneElement
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
	//bool PreUpdate();

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

public:
	SDL_Texture* player_texture;
	SDL_Texture* maptex;
	SDL_Texture* hit_tex;
	SDL_Rect Rect_player;
	std::string tex_player_file_name;
	std::string texmapfile_name;
	std::string file_hit;

	uint gems, bombs, arrows,charge;
private:
	Text* hp_text;
	Text* attack_text;
	int attack;

	std::list<Item*> bag;

	Collider* collision_player;

	//TEST
	std::string stats_temp;

	bool Camera_follow_player;
	bool changeResolution;

};





#endif