#ifndef __j1SCENEINTRO_H__
#define __j1SCENEINTRO_H__

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

class j1SceneIntro : public j1Module
{
public:

	j1SceneIntro();

	// Destructor
	virtual ~j1SceneIntro();

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
	//Start screen
	SDL_Texture* TitleScreen_letters;
	SDL_Texture* TitleScreen_bg;
	float bg_anim = 0;
	bool right = false;

};

#endif