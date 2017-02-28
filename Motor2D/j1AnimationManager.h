#pragma once
#ifndef __j1ANIMATION_H_
#define __j1ANIMATION_H_

#include "j1Module.h"
#include <vector>
#include "Animation.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Defs.h"

enum AnimationState{IDLE,WALKING,ATTACK};
enum Direction{UP,DOWN,LEFT,RIGHT};

class j1AnimationManager : public j1Module
{
public:
	j1AnimationManager();

	// Destructor
	~j1AnimationManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void PlayerSelector(AnimationState status, Direction dir, iPoint position);

private:
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	std::vector<Animation*> link_anim;
	Animation link_walk_east;
	Animation link_walk_north;
	Animation link_walk_west;
	Animation link_walk_south;
	Animation* current_animation;
	SDL_Texture* graphics = nullptr;

	int range_link;

	std::string file_texture;
};

#endif //__j1ANIMATION_H_