#ifndef __j1GUIELEMENTS_H__
#define __j1GUIELEMENTS_H__

#include "j1Module.h"
#include "j1InputManager.h"

enum ButtonState { normal, over, clicked };
enum FontName { GANONF, PIXEL, PIXELMORE, POKE1 };


struct 	_TTF_Font;
class Animation;
class InputManager;
class PokemonCombat;
// ---------------------------------------------------

class Image : public j1GuiEntity
{
public:
	void Update(j1GuiEntity* focused);
	void Draw();

public:
	Image(SDL_Rect rectangle, iPoint position, std::string identifier, GuiGroup group);
	~Image();
	void AssignNumber(uint n);
public:
	bool start = false;


};

// ----------------------------------------------------

class Button : public j1GuiEntity
{
public:
	void Draw();
	void Update(j1GuiEntity* focused);
public:
	Button(SDL_Rect rectangle, iPoint pos, iPoint stat2, iPoint stat3, bool animated, std::string identifier, GuiGroup group, const char* textstring, uint textsize, iPoint textpos);
	~Button();

public:
	bool start = true, click = false;
	ButtonState state = normal;
	SDL_Rect texture2 = { 0,0,0,0 }, texture3 = { 0,0,0,0 };
	Text* buttontext = nullptr;;
	Animation* anim = nullptr;
};

// -----------------------------------------------------

class Text : public j1GuiEntity
{
public:
	Text(FontName search, const char* write, SDL_Color color, uint length, iPoint pos, uint size, bool draw, std::string identifier, GuiGroup group);
	~Text();

public:
	void Draw();
	void Update(j1GuiEntity* focused);
	void Write(const char* string); // adapt to write paragrahps
	void CheckString(std::string string);
	void Visible(bool yes);
private:
	std::string text;
	SDL_Texture* text_texture = nullptr;
	_TTF_Font* font = nullptr;
public:
	uint length = 50, size = 30;
	Text* next_line = nullptr;
	FontName font_name = GANONF;
	SDL_Color color = { 255,255,255,255 };
	bool draw = true;
};

// -------------------------------------------------------

class Dialogue : public j1GuiEntity
{
public:
	Dialogue(const char* string);
	~Dialogue();
public:
	void Draw();
	void Update(j1GuiEntity* focused);
	void PushLine(bool push);
	void Clear();
public:
	float diferential = 0.5;
	Text* lines = nullptr;
	bool push = false;
	uint timer = 0;
	Selector* options = nullptr;
	bool end = false;
	uint pushes = 0; //8 TOP
	uint enters = 0;
};

class Selector : public j1GuiEntity
{
public:
	Selector(const char* first_option, const char* second_option, j1GuiEntity* parent);
	~Selector();
public:
	void Handle_Input();
private:
	Text* first = nullptr, *second = nullptr;
	Text* selector = nullptr;
	bool position = true;//true up false down
};
// ------------------------------------------------------
class MainMenu : public j1GuiEntity, public InputListener
{
public:
	MainMenu();
	~MainMenu();
public:
	void Input();
	void Select(bool next); //true for next, false for prev
	Button* GetElement(uint id);
	void OnInputCallback(INPUTEVENT action, EVENTSTATE e_state);
private:
	std::vector<Button*>options;
public:
	bool active = false;
	//placeholder for create/save game and name editor
};

// ------------------------------------------------------
class ZeldaHud : public j1GuiEntity
{
public:
	ZeldaHud();
	~ZeldaHud();
public:
	void Update(j1GuiEntity* focused);
	void AssignValues(Image* assigner, uint var);
	void Move(bool axis, float speed, bool move_all = false);
	void Input();
	// TU DU heart container manager
	void Equip(const char* item);
public:
	Image* charge = nullptr;
	Image* force = nullptr;
	Image *gems = nullptr;
	Image *bombs = nullptr;
	Image *arrows = nullptr;
	Image *hp = nullptr;
	Image *picked = nullptr; // set R1 and L1 interaction to change items inhud without opening menu
};

/////////////////////////////// MENU ITEMS /////////////////////////////////
struct ItemMenu
{
	ItemMenu(Button* button)
	{
		ui_button = button;
	}
	Button *ui_button = nullptr;
	bool picked = false;
	std::string description;
};

class ZeldaMenu : public j1GuiEntity, public InputListener
{
public:
	ZeldaMenu();
	~ZeldaMenu();
public:
	void Update(j1GuiEntity* focused);
	void Input();
	void OnInputCallback(INPUTEVENT, EVENTSTATE); //TODO LOW -> check if its better to put button inputs here or not
public:
	void ResetInventory();
	void Select(bool next); //true for next, false for previous
	void Move(bool axis, float speed);
	void Equip(const char* item);
	void ShowItemInfo();
	void PickItem(const char* name);
	void SelectOption(bool down);
	Button* GetFirst(bool option); //bool option on true, for getting first element on option mini menu instead of the items list
private: 
	std::vector<ItemMenu*>items;
	std::vector<Button*>options;
	Image* item_info, *item_equiped;
	Text* item_info_name, *item_eq_name, *item_info_desc;
	bool empty = true;
public:
	bool on_options = false;
	bool active = false;
};

//----------------------------------------------------------

class PokemonCombatHud :public j1GuiEntity
{
public:
	PokemonCombatHud(PokemonCombat* Link, PokemonCombat* Brendan);
	~PokemonCombatHud();
public:
	void Input();
	void Update(j1GuiEntity* focused);
public:
	void SetCd(uint newcd);
	//void OpenClose(bool open);//true open false close
	void Move(bool x_axis, float speed);
	void LoadNewPokemon(PokemonCombat* pokemon, bool trainer);
	void CombatInfo(PokemonCombat* pokemon_1, PokemonCombat* pokemon_2);
	void GetDamage(uint damage, bool trainer);
public:
	bool cooldown = false;
	iPoint cdtime = { 0,0 }; //.X SHOWS CD TIME AND .Y SHOWS REMAINING CD TIME

private:
	//std::vector<Image*>hud_images;
	Image* ability = nullptr;
	Image* hp1 = nullptr;
	Image* hp2 = nullptr;
	Text* poke_hp_Link = nullptr;
	Text* poke_hp_Brendan = nullptr;
	iPoint hpbar_pLink = { 0,0 };//.X SHOWS MAX HP, .Y SHOWS ACTUAL LIFE (SAME AS ABOVE) used for triangle operations with atlas pixels
	iPoint hpbar_pBrendan = { 0,0 };
	char buffer[25]; //hp
	iPoint num_pokemons = { 0,0 };

};

//---------------------------------------

class PokemonWorldHud :public j1GuiEntity
{
public:
	PokemonWorldHud();
	~PokemonWorldHud();
public:

};
#endif // __j1GUIELEMENTS_H__