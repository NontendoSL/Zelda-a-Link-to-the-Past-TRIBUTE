#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1EntityElementsScene.h"

#define FIRST_LEVEL_COMBAT 17

enum GameState { INGAME, INMENU, TIMETOPLAY, GAMEOVER, CUTSCENE};

struct SDL_Texture;

class j1GuiEntity;
class Image;
class Text;
class Button;
class Dialogue;
class ZeldaHud;
class ZeldaMenu;
class PokemonCombatHud;
class PokemonWorldHud;
class PokemonWorldMenu;
class PokemonWorldBag;
class PokemonWorldShop;
class ControllerMapping;

class Player;
class Item;
class Soldier;
class DynamicObjects;
class PokemonCombat;
enum WeaponType;

struct Teleport
{
	std::string name;
	SDL_Rect rect;
	int id_map;
	iPoint position;
};

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

	//OnGui buttons
	void OnGui(j1GuiEntity* element, GuiAction event);

	//Create Teleports
	void CreateTeleports();

	//Load Maps
	bool Load_new_map(int n, bool isTP);
	//Load CombatMaps
	bool Load_Combat_map(int n);

	void SwitchMenu(bool direction);

	void ChangeState(GameState state);

	//Load maps function
	void SwitchMap(bool isTP);

	//SAVE/LOAD FUNCTION
	bool Save(pugi::xml_node&) const;

private:
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	void LoadUi();
	void GoMainMenu();

public:
	SDL_Texture* debug_tex;
	GameState gamestate = INGAME;
	//Teleports
	std::vector<Teleport> teleports;

	//Combat or not
	bool combat = false;
	//Ingame
	ZeldaHud* hud = nullptr;
	ZeldaMenu* start_menu = nullptr;
	PokemonCombatHud* pokecombat = nullptr;
	PokemonWorldHud* poke_hud = nullptr;
	PokemonWorldMenu* poke_menu = nullptr;
	PokemonWorldBag* poke_bag = nullptr;
	PokemonWorldShop* poke_shop = nullptr;
	Player* player = nullptr;
	PokemonCombat* poke_enemy = nullptr;

	pugi::xml_node		config;

	iPoint newPosition;

	bool ingame = false;
	bool switch_menu = false;
	bool inventory = false;

	//id maps -------------------------------
	int switch_map = 0;
	int last_map = 0;
	int combat_map_id = 0;

	bool notrepeatmusic;

	bool fade = false;
	bool now_switch = false;
	WeaponType weapon_equiped;

	//ONLY Release 0.7
	bool goPokemon = false;
	bool dialog_inmapZelda = false;

	////DIALOGUE VICENTE METHOD PROVISIONAL TILL WE IMPLEMENT IT PROPERLY
	uint help_timer = 0;
	bool help_bool = true;

	//provisional app close and game loop
	bool update_return = true;
	bool first_loop = true;
	bool useTP = false;
	bool notrepeatCombat = true;

	bool swap_object = false;
	bool swap_ganon = false;
	bool swap_player = false;

	bool cash_swapped = false;
private:
	uint volume = 120;
	bool joy_talk = false;

};

#endif // __j1SCENE_H__