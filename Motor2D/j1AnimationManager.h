#pragma once
#ifndef __j1ANIMATION_H_
#define __j1ANIMATION_H_

#include "j1Module.h"
enum AnimationState{IDLE,WALKING,ATTACK};
enum Direction{TOP,BOT,LEFT,RIGHT};

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

	void PlayerSelector(AnimationState status, Direction dir);
private:


};

#endif //__j1ANIMATION_H_