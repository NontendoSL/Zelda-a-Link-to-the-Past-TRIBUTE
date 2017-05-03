#ifndef __j1GUIENTITY_H__
#define __j1GUIENTITY_H__

#include "j1Module.h"

enum GuiAction
{
	PLACEHOLDER,
	CLICK_DOWN,
	CLICK_UP
};
enum GuiGroup;
class j1Module;
// --------------------------------------------------- 
class j1GuiEntity //: public j1Gui
{
public:

	j1GuiEntity();
	j1GuiEntity(SDL_Rect rectangle, iPoint position, std::string identifier = "undefined", bool resize = true, GuiGroup group = GuiGroup::NONE);

	// Destructor
	virtual ~j1GuiEntity();

	virtual void Update(j1GuiEntity* focused);

	virtual void Draw();

	virtual void AssignNumber(uint n);

	void CalculateDiferential();

public:
	GuiType type;
	SDL_Rect Hitbox;
	iPoint position, diferential;
	bool visible = true;
	bool resize = true;
	bool selected = false;
	bool focusable = false;
	bool movable = true;
	std::string identifier;
	std::vector<j1GuiEntity*> elements;
	j1GuiEntity* parent;
	j1Module* listener = nullptr;
	GuiGroup belong;
};

#endif // __j1GUIENTITY_H__