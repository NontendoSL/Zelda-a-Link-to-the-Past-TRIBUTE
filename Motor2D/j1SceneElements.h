#pragma once
#pragma once
#ifndef __j1SCENEELEMENT_H__
#define __j1SCENEELEMENT_H__

#include "j1Module.h"
#include <list>



// ---------------------------------------------------
class j1SceneElement : public j1Module
{
public:

	j1SceneElement()
	{}

	~j1SceneElement()
	{}

	// Called when before render is available
	virtual bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	virtual bool Update(float dt);

	// Called before quitting
	bool CleanUp();





};

#endif // __j1SCENEELEMENT_H__