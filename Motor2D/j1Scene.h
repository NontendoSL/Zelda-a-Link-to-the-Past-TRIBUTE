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
class Menu;

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

	bool Load_new_map(int n);

	void AssignValues(Image* assigner,uint var);

	void SwitchMenu(bool direction);
private:
	SDL_Texture* debug_tex;
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	void LoadUi();

public:


	//Ingame
	Menu* hud=nullptr,*start_menu=nullptr;
	Image* force, *gems, *bombs, *arrows;
	Player* player = nullptr;
	std::list<Soldier*> enemy; //TODO LOW -> change to std::list<NPC*> enemies;
	std::list<Item*> items;
	std::list<DynamicObjects*> dynobjects;
	Dialogue* dialog = nullptr;

	pugi::xml_node		config;

	bool ingame = false, switch_menu=false, inventory=true;
	int switch_map = 0;
};

#endif // __j1SCENE_H__