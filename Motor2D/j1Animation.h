#pragma once
#ifndef __j1ANIMATION_H_
#define __j1ANIMATION_H_

#include "j1Module.h"


class j1Animation : public j1Module
{
public:
	j1Animation();

	// Destructor
	~j1Animation();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);


	// Called before quitting
	bool CleanUp();


private:


};





#endif //__j1ANIMATION_H_