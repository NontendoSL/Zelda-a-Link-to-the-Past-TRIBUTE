#pragma once
#ifndef __SCEPTYLE_H_
#define __SCEPTYLE_H_

#include "Pokemon.h"

class Sceptyle : public Pokemon
{
public:
	Sceptyle();

	~Sceptyle();

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

#endif //__SCEPTYLE_H_