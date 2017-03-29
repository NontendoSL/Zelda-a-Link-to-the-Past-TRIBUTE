#pragma once
#ifndef __BLAZIKEN_H_
#define __BLAZIKEN_H_

#include "Pokemon.h"

class Blaziken : public Pokemon
{
public:
	Blaziken();

	~Blaziken();

	// Called before render is available
	bool Awake(pugi::xml_node&, uint);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// Called before quitting
	bool CleanUp();

	/*void AddItem(Item*);

	void Drop_item();*/

private:



};

#endif //__BLAZIKEN_H_