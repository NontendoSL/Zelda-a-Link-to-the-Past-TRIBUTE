#pragma once
#ifndef __j1ANIMATION_H_
#define __j1ANIMATION_H_

#include "j1Module.h"
#include <vector>
#include "Animation.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Defs.h"

enum ActionState;
enum Direction;

struct AnimDirect
{
	Animation East_action;
	Animation North_action;
	Animation West_action;
	Animation South_action;
};

struct AnimationStruct
{
	std::vector<AnimDirect> anim;
	std::string name;
	SDL_Texture* graphics = nullptr;
};

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

	// Called before quitting
	bool CleanUp();

	void Drawing_Manager(ActionState status, Direction dir, iPoint position, std::string name);

	Direction CheckDir(Direction);

private:
	pugi::xml_node LoadConfig(pugi::xml_document& config_file, std::string file) const;
	//All Files
	std::list<std::string> file_names;

	//All animations of the game
	std::vector<AnimationStruct> animat;

	int range_link;
};

#endif //__j1ANIMATION_H_