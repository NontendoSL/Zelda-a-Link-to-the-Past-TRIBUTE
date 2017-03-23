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
private:
	SDL_Texture* debug_tex;
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	void LoadUi();

public:


	//Ingame
	Image* charge = nullptr,* force = nullptr;
	Image* item = nullptr;
	Image* gems = nullptr;
	Image* bombs = nullptr;//TODO LOW: Agrupar imatges bomba, rupias vida i fletxes en una sola Imatge*
	Image* arrows = nullptr; //TODO HIGH: Organize all this in less code;
	Image* life = nullptr;
	Text* test = nullptr;
	Player* player = nullptr;
	std::list<Soldier*> enemy;
	std::list<Item*> items;
	std::list<DynamicObjects*> dynobjects;
	Dialogue* dialog = nullptr;

	pugi::xml_node		config;

	bool ingame = false;
	int switch_map = 0;
};

#endif // __j1SCENE_H__