#pragma once
#ifndef __j1ANIMATION_H_
#define __j1ANIMATION_H_

#include "j1Module.h"
#include <vector>
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Defs.h"
#include "Animation.h"

enum AnimationElement //TODO HIGH-> ORDER THIS ENUM
{
	LINK,
	SOLDIER,
	
	//WEAPONS ---------
	HOOKSHOT,
	BOMB,

	GEODUDE,
	GOLEM,
	//GOLBAT,
	SUDOWOODO,
	TRAINER,
	BC_TROOPER,
	GANON,

	//POKÉMON COMBAT ---------
	BLAZIKEN, SWAMPERT, SCEPTILE,
	//CRAWDAUNT, ABSOL, SHIFTRY,
	//DUSCLOPS, BANETTE, SABLEYE,
	//SEALEO, WALREIN, GLALIE,
	//SALAMENCE, FLYGON, ALTARIA,
	//MEWTWO, GROUDON, RAIKOU
};

enum Direction;

struct AnimDirect
{
	Animation East_action;
	Animation North_action;
	Animation West_action;
	Animation South_action;

	void ResetAnimations();
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

	void Drawing_Manager(uint state_id, Direction dir, iPoint position, AnimationElement element);

	Animation* GetAnimation(uint state_id, Direction dir, AnimationElement element);

	AnimationStruct* GetAnimStruct(AnimationElement element);

private:
	pugi::xml_node LoadConfig(pugi::xml_document& config_file, std::string file) const;

	//All Files
	std::list<std::string> file_names;

	//All animations of the game
	std::vector<AnimationStruct> animat;

	iPoint pivot {0,0};
	SDL_Rect r;

	//UITILITY POINTERS ------------------------
	Animation* current_animation = nullptr;
	AnimationStruct* anim_struct = nullptr;
	//------------------------------------------

};

#endif //__j1ANIMATION_H_