#pragma once
#ifndef __SUDOWOODO_H_
#define __SUDOWOODO_H_

#include "Pokemon.h"

class Sudowoodo : public Pokemon
{
public:
	Sudowoodo();

	~Sudowoodo();

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

#endif //__SUDOWOODO_H_