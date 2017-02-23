#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1EntityElementsScene.h"

struct SDL_Texture;

class j1GuiEntity;
class Image;
class Text;
class Button;
class TextBox;

class Player;
class Item;
class Enemy;

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


private:
	SDL_Texture* debug_tex;

public:
	Image* charge;
	Image* item;
	Image* gems;
	Text* test;
	Player* player;
	Enemy* enemy;
	Item* items;

};

#endif // __j1SCENE_H__