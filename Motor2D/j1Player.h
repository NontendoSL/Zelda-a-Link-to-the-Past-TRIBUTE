#pragma once
#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "j1SceneElements.h"
#include "SDL/include/SDL_rect.h"

class Text;

class Player : public j1SceneElement
{
public:
	Player(iPoint position);

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

	// Called before all Updates
	//bool PostUpdate();

	// Called before quitting
	bool CleanUp();


private:

	SDL_Texture* player_texture;
	SDL_Texture* maptex;
	SDL_Rect Rect_player;
	std::string tex_player_file_name;
	std::string texmapfile_name;

	Text* hp_text;
	int hp;
	Text* attack_text;
	int attack;

	//TEST
	std::string stats_temp;

	bool Camera_follow_player;
	bool changeResolution;

};





#endif