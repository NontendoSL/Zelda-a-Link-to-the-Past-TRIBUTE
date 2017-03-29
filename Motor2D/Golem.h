#pragma once
#ifndef __GOLEM_H_
#define __GOLEM_H_

#include "Pokemon.h"

class Golem : public Pokemon
{
public:
	Golem();

	~Golem();

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

#endif //__GOLEM_H_