#pragma once
#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "j1Module.h"


class j1Player : public j1Module
{
public:
	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	//bool PostUpdate();

	// Called before quitting
	bool CleanUp();


private:

	iPoint pos;
	SDL_Texture* player_texture;
	SDL_Texture* maptex;
	SDL_Rect Rect_player;
	std::string tex_player_file_name;
	std::string texmapfile_name;

	bool Camera_follow_player;
	bool changeResolution;

};





#endif