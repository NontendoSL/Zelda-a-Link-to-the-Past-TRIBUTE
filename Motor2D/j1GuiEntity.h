#ifndef __j1GUIENTITY_H__
#define __j1GUIENTITY_H__

#include "j1Module.h"



// ---------------------------------------------------
class j1GuiEntity //: public j1Gui
{
public:

	j1GuiEntity();
	j1GuiEntity(SDL_Rect rectangle, iPoint position,std::string identifier="undefined", uint id=0);

	// Destructor
	virtual ~j1GuiEntity();

	virtual void Update();

	virtual void Draw();

	virtual void Handle_Input();

public:

	GuiType type;
	SDL_Rect Hitbox;
	iPoint position, diferential;
	bool visible = true;
	bool selected;
	uint id;
	std::string identifier;
private:

};

#endif // __j1GUIENTITY_H__