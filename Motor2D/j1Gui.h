#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2DynArray.h"

#define CURSOR_WIDTH 2
enum GuiType { BUTTON, TEXT, TEXT_BOX, MOUSE, IMAGE };

// TODO 1: Create your structure of classes
class j1GuiEntity;
class Image;
class Text;
class Button;
class TextBox;

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	const SDL_Texture* GetAtlas() const;

	// create img
	Image* CreateImage(SDL_Rect rect,iPoint pos);
	// create text
	Text* CreateText(const char* string, iPoint pos, uint size);
	// create button
	Button* CreateButton(SDL_Rect rect, iPoint pos, iPoint text2, iPoint text3, const char* textstring = nullptr, uint textsize = NULL, iPoint textpos = { 0,0 });


public:
	int Next_id = 0;
private:

	p2SString atlas_file_name;
	p2DynArray<j1GuiEntity*> entities;

protected:
	SDL_Texture* atlas;

};

#endif // __j1GUI_H__