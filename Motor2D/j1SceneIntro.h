#ifndef __j1SCENEINTRO_H__
#define __j1SCENEINTRO_H__

#include "j1Module.h"
#include "j1EntityElementsScene.h"
#include "j1InputManager.h"

struct SDL_Texture;

class j1GuiEntity;
class Image;
class Text;
class Button;
class Dialogue;
class MainMenu;
class Player;
class Item;
class Soldier;
class DynamicObjects;
class InputListener;

class j1SceneIntro : public j1Module, public InputListener
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

	void LoadMainMenu();

	//void LoadNewMap(int id);

	void OnGui(j1GuiEntity* element, GuiAction event);

private:
	//Start screen
	SDL_Texture*	TitleScreen_letters = nullptr;
	SDL_Texture*	TitleScreen_bg = nullptr;
	SDL_Texture*	Menu_bg = nullptr;
	float			bg_anim = 0;
	bool			right = false;


	uint Menu_Cursor;
	uint volume = 120;

public:
	MainMenu*			main_menu = nullptr;
	ControllerMapping*  controller_map = nullptr;
	bool				goHouse = false;
	bool				fade;
	bool				menu = false;
};

#endif