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
class Enemy;
class DynamicItems;

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

	void AssignValues(Image* assigner);
private:
	SDL_Texture* debug_tex;
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;

public:
	Image* charge;
	Image* item;
	Image* gems;
	Image* bombs;//TODO LOW: Agrupar imatges bomba, rupias vida i fletxes en una sola Imatge*
	Image* arrows; //TODO MID: Group all ui in lists
	Image* life;
	Text* test;
	Player* player;
	std::list<Enemy*> enemy;
	std::list<Item*> items;
	std::list<DynamicItems*> dynitems;
	Dialogue* dialog;

	pugi::xml_node		config;


	int switch_map;
};

#endif // __j1SCENE_H__