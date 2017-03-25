#ifndef __j1GUIENTITY_H__
#define __j1GUIENTITY_H__

#include "j1Module.h"



// ---------------------------------------------------
class j1GuiEntity //: public j1Gui
{
public:

	j1GuiEntity();
	j1GuiEntity(SDL_Rect rectangle, iPoint position,std::string identifier="undefined", uint id=0,bool resize=true);

	// Destructor
	virtual ~j1GuiEntity();

	virtual void Update();

	virtual void Draw();

	virtual void Handle_Input();

	virtual void AssignNumber(uint n);

public:

	GuiType type;
	SDL_Rect Hitbox;
	iPoint position, diferential;
	bool visible = true, selected, resize;
	uint id;
	std::string identifier;
	std::vector<j1GuiEntity*> elements;
};

#endif // __j1GUIENTITY_H__