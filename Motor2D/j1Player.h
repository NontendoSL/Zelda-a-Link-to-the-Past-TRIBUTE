#pragma once
#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "j1SceneElements.h"


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

	void Draw();
	// Called before all Updates
	//bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Save();


public:
	SDL_Texture* player_texture;
	SDL_Texture* maptex;
	SDL_Rect Rect_player;
	std::string tex_player_file_name;
	std::string texmapfile_name;

private:
	Text* hp_text;
	Text* attack_text;
	int attack;

	std::list<Item*> bag;

	//TEST
	std::string stats_temp;

	bool Camera_follow_player;
	bool changeResolution;

};





#endif