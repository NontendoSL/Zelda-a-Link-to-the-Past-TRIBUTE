#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1EntityElementsScene.h"

#define FIRST_LEVEL_COMBAT 17

enum GameState {INGAME, INMENU, TIMETOPLAY, CUTSCENE, GAMEOVER};

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

struct CheckPointInfo
{
	//MAP
	iPoint pos = { 0, 0 };
	int map_id = 0;

	//UI
	std::string world;

	//ZELDA WORLD ---------------
	//STATS
	iPoint hp_hearts{ 6, 6 };

	//RESOURCES
	uint rupees = 0;
	uint bombs = 0;
	uint arrows = 0;

	//WEAPONS
	bool bow_picked = false;
	bool bombcontainer_picked = false;
	bool hookshot_picked = false;
	bool sword_picked = false;
	//-----------------------------

	//POKEMON WORLD --------
	uint cash = 0;
	int blaz_hp = 0;
	int blaz_atk = 0;
	int blaz_def = 0;
	int blaz_pos = 0;
	int scept_hp = 0;
	int scept_atk = 0;
	int scept_def = 0;
	int scept_pos = 1;
	int swamp_hp = 0;
	int swamp_atk = 0;
	int swamp_def = 0;
	int swamp_pos = 2;
	//----------------------
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
	bool NewGame();
	bool ContinueGame();
	bool Load_new_map(int n, bool isTP);

	//Load CombatMaps
	bool Load_Combat_map(int n);

	void SwitchMenu(bool direction);

	void ChangeState(GameState state);

	//Load maps function
	void SwitchMap(bool isTP);

	//SAVE/LOAD FUNCTION
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	void LoadFX();

	void SetItemsinUI();

	void SetMusics(int id);

	CheckPointInfo Check;
	void GoMainMenu();
private:
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	void LoadUi();


public:
	SDL_Texture* debug_tex;
	GameState gamestate = INGAME;
	GameState gamestate_input = INMENU;
	//Teleports
	std::vector<Teleport> teleports;

	//Combat or not
	bool combat = false;
	//video
	bool playVideo = false;
	bool waitVideo = false;
	//ModeGod
	bool modeGod = false;
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

	// key
	bool key_boss = false;
	// Life
	bool Link_die = false;

	//TimePause
	j1TimeNormal timepause;
	uint32 timepaused32;
	bool stop = false;

	//id maps -------------------------------
	int switch_map = 0;
	int last_map = 0;
	int combat_map_id = 0;
	int cutscene_id = -1;

	bool notrepeatmusic;

	bool fade = false;
	bool now_switch = false;
	WeaponType weapon_equiped;

	//ONLY Release 0.7
	bool goPokemon = false;
	bool dialog_inmapZelda = false;

	////DIALOGUE VICENTE METHOD PROVISIONAL TILL WE IMPLEMENT IT PROPERLY
	uint help_timer = 0;
	bool help_bool = false;

	//provisional app close and game loop
	bool update_return = true;
	bool first_loop = true;
	bool useTP = false;
	bool notrepeatCombat = true;

	//sleep
	bool sleep_start = false;

	bool swap_object = false;
	bool swap_ganon = false;
	bool swap_player = false;
	bool use_tatical = false;

	bool cash_swapped = false;

	//NEW GAME / CONTINUE
	bool new_game = false;
	bool continue_game = false;
	uint win_timer = 0;
	bool joy_talk = false;
private:
	uint volume = 50;


};

#endif // __j1SCENE_H__