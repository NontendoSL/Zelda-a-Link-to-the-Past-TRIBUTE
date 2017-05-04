#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
//#include "p2DynArray.h"
#include <vector>

#define CURSOR_WIDTH 2
enum GuiType { BUTTON, TEXT, TEXT_BOX, MOUSE, IMAGE, DIALOGUE, MENU };
enum GuiGroup
{
	NONE,
	MAIN_MENU,
	ZELDA_HUD,
	ZELDA_MENU,
	POKEMON_COMBAT
};
enum FontName;

class j1GuiEntity;
class Image;
class Text;
class Button;
class Dialogue;
class MainMenu;
class ZeldaHud;
class ZeldaMenu;
class PokemonCombatHud;
class Pokemon;
class Selector;

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

	void ReceiveInput();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	const SDL_Texture* GetAtlas() const;

	// create img
	Image* CreateImage(SDL_Rect rect, iPoint pos, std::string identifier = "undefined", GuiGroup group = GuiGroup::NONE, bool movable = true);
	// create text
	Text* CreateText(FontName search, const char* string, uint length, iPoint pos, uint size, SDL_Rect  color = { 255,255,255,255 }, bool addelement = true, std::string identifier = "undefined", GuiGroup group = GuiGroup::NONE);
	// create button
	Button* CreateButton(j1Module* listener, SDL_Rect rect, iPoint pos, iPoint text2, iPoint text3, bool animated = false, std::string identifier = "undefined", GuiGroup group = GuiGroup::NONE, const char* textstring = nullptr, uint textsize = NULL, iPoint textpos = { 0,0 });

	Dialogue* CreateDialogue(const char* string);

	Selector* CreateSelector(const char* first, const char* second, j1GuiEntity*parent);

	MainMenu* CreateMainMenu();

	ZeldaHud* CreateZeldaHud();

	ZeldaMenu* CreateZeldaMenu();

	PokemonCombatHud* CreatePokemonCombatHud(Pokemon* Link, Pokemon* Brendan);

	void Erase(j1GuiEntity* to_delete);

	int GetEntitiesSize();

	j1GuiEntity* GetFocused();

	void SetFocus(j1GuiEntity* to_focus);

	void MoveGroup(GuiGroup group, bool x_axis, float speed, bool move_all = false);

	void SetGui(GuiGroup guistate);

	pugi::xml_node j1Gui::LoadConfig(pugi::xml_document& config_file, std::string file) const;

	void LoadEntities();

	j1GuiEntity* GetEntity(const char* identifier);
public:
	int Next_id = 0;

private:
	std::string atlas_file_name;
	std::string entities_file_name;
	std::vector<std::string> gui_groups_name;
	std::vector<j1GuiEntity*> entities;
	SDL_Texture* atlas = nullptr;
	j1GuiEntity* focused = nullptr;
	GuiGroup status = NONE;

	//friend class ZeldaMenu; // work on this, search for alternative option
};

#endif // __j1GUI_H__