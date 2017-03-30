#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
//#include "p2DynArray.h"
#include <vector>

#define CURSOR_WIDTH 2
enum GuiType { BUTTON, TEXT, TEXT_BOX, MOUSE, IMAGE, DIALOGUE,MENU };
enum FontName;

// TODO 1: Create your structure of classes
class j1GuiEntity;
class Image;
class Text;
class Button;
class Dialogue;
class ZeldaMenu;
class PokemonCombatHud;

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
	Image* CreateImage(SDL_Rect rect, iPoint pos, std::string identifier="undefined", uint id=0);
	// create text
	Text* CreateText(FontName search, const char* string, uint length, iPoint pos, uint size, SDL_Color color = { 255,255,255,255 }, bool addelement = true, std::string identifier = "undefined", uint id = 0);
	// create button
	Button* CreateButton(SDL_Rect rect, iPoint pos, iPoint text2, iPoint text3, bool animated = false, std::string identifier = "undefined", uint id = 0, const char* textstring = nullptr, uint textsize = NULL, iPoint textpos = { 0,0 });

	Dialogue* CreateDialogue(const char* string);

	ZeldaMenu* CreateZeldaMenu();

	PokemonCombatHud* CreatePokemonCombatHud(uint cd_time, uint hpbar1, uint hpbar2);

public:
	int Next_id = 0;

private:
	std::string atlas_file_name;
	//p2DynArray<j1GuiEntity*> entities;
	std::vector<j1GuiEntity*> entities;

protected:
	SDL_Texture* atlas;

};

#endif // __j1GUI_H__