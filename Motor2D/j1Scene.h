#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1EntityElementsScene.h"

struct SDL_Texture;

class j1GuiEntity;
class Image;
class Text;
class Button;
class Dialogue;
class ZeldaMenu;
class PokemonCombatHud;

class Player;
class Item;
class Soldier;
class DynamicObjects;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Load Maps
	bool Load_new_map(int n);
	//Load CombatMaps
	bool Load_Combat_map(int n);

	void AssignValues(Image* assigner,uint var);

	void SwitchMenu(bool direction);
private:
	SDL_Texture* debug_tex;
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	void LoadUi();

public:

	//Combat or not
	bool combat = false;
	//Ingame
	ZeldaMenu* hud=nullptr,*start_menu = nullptr;
	PokemonCombatHud* pokecombat = nullptr;
	Image* force, *gems, *bombs, *arrows;
	Player* player = nullptr;
	std::list<Soldier*> enemy; //TODO LOW -> change to std::list<NPC*> enemies;
	std::list<Item*> items;
	std::list<DynamicObjects*> dynobjects;
	Dialogue* dialog = nullptr;

	pugi::xml_node		config;

	bool ingame = false;
	bool switch_menu = false;
	bool inventory = false;
	int switch_map = 0;

	bool fade = false;
	bool now_switch = false;
};

#endif // __j1SCENE_H__