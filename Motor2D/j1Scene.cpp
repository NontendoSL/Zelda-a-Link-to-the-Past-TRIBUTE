#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1GuiEntity.h"
#include "j1GuiElements.h"
#include "j1Scene.h"
#include "Soldier.h"
#include "j1Player.h"
#include "j1DynamicObjects.h"
#include "j1FileSystem.h"
#include "CombatManager.h"
#include "j1FadeToBlack.h"
#include "PokeTrainer.h"
#include "j1Collision.h"
#include "j1Weapon.h"

j1Scene::j1Scene() : j1Module()
{
	name="scene";
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	ingame = false;
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if (ingame == true)
	{
		LoadUi();
		Load_new_map(1);
		App->audio->PlayMusic("audio/music/ZELDA/Zeldakakariko_village.ogg");
		App->audio->VolumeMusic(85);
		App->audio->LoadFx("audio/fx/LTTP_Pause_Open.wav"); //2
		App->audio->LoadFx("audio/fx/LTTP_Pause_Close.wav"); //3
		App->audio->LoadFx("audio/fx/LTTP_Rupee1.wav");//4
		App->audio->LoadFx("audio/fx/LTTP_Sword_Attack.wav");//5
		App->audio->LoadFx("audio/fx/LTTP_Bomb_Lay.wav");//6
		App->audio->LoadFx("audio/fx/LTTP_Bomb_Explosion.wav");//7
		App->audio->LoadFx("audio/fx/PKMN_Scratch_Attack.wav");//8
		App->audio->LoadFx("audio/fx/PKMN_Blaziken_Attack.wav");//9
		App->audio->LoadFx("audio/fx/PKMN_Tackle_Attack.wav");//10
		App->audio->LoadFx("audio/fx/LTTP_Enemy_Death.wav");//11
		App->audio->LoadFx("audio/fx/LTTP_Enemy_Hit.wav");//12
		App->audio->LoadFx("audio/fx/LTTP_Link_Hurt.wav");//13
		App->audio->LoadFx("audio/fx/LTTP_Fall.wav");//14
		App->audio->LoadFx("audio/fx/LTTP_Chest_Open.wav");//15
		

	}
	inventory = false;
	switch_map = 0;
	notrepeatmusic = true;


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{



	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Update_Scene", Profiler::Color::DarkGreen)

		if (ingame == true)
		{
			//TODO AssignValues to only interact when picking/droping items
			AssignValues(gems, player->gems);
			AssignValues(bombs, player->bombs);
			AssignValues(arrows, player->arrows);
			/*if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) { // provisional heart/damage checker
				player->GetDamage();
			}
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) { // provisional heart container add
				player->AddHeartContainer();
			}*/

			player->ShowHearts();
			force->Hitbox.w = player->charge;

			if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
			{
				switch_map = 7;
			}

			if (switch_map == 7)
			{
				if (fade == false)
				{
					App->fadetoblack->FadeToBlack();
					fade = true;
					now_switch = true;
				}

				if (App->fadetoblack->Checkfadetoblack() && now_switch)
				{
					combat = true;
					now_switch = false;
					if (App->map->CleanUp())
					{
						App->collision->EreseAllColiderPlayer();
						if (player->equiped_item != nullptr)
						{
							weapon_equiped = player->equiped_item->Wtype;
						}
						App->entity_elements->DelteElements();
						if (enemy.size() > 0)
						{
							enemy.clear();
						}
						if (items.size() > 0)
						{
							items.clear();
						}
						if (dynobjects.size() > 0)
						{
							dynobjects.clear();
						}
						if (pokemons.size() > 0)
						{
							pokemons.clear();
						}
						if (poketrainer.size() > 0)
						{
							poketrainer.clear();
						}
						Load_Combat_map(7);
					}
				}
				if (App->fadetoblack->Checkfadefromblack())
				{
					fade = false;
					switch_map = 0;
				}
			}

			if (switch_map != 0)
			{
				if (fade == false)
				{
					App->fadetoblack->FadeToBlack();
					player->gamestate = INMENU;
					player->state = IDLE;
					fade = true;
					now_switch = true;
				}

				if (App->fadetoblack->Checkfadetoblack() && now_switch)
				{
					now_switch = false;
					if (App->map->CleanUp())
					{
						App->collision->EreseAllColiderPlayer();
						if (player->equiped_item != nullptr)
						{
							weapon_equiped = player->equiped_item->Wtype;
						}
						App->entity_elements->DelteElements();
						if (enemy.size() > 0)
						{
							enemy.clear();
						}
						if (items.size() > 0)
						{
							items.clear();
						}
						if (dynobjects.size() > 0)
						{
							dynobjects.clear();
						}
						if (pokemons.size() > 0)
						{
							pokemons.clear();
						}

						Load_new_map(switch_map);
						id_map = switch_map;

						if (switch_map == 4 && notrepeatmusic)
						{
							notrepeatmusic = false;
							App->audio->PlayMusic("audio/music/POKEMON/PokemonVictoryRoad.ogg");
						}
					}
				}
				if (App->fadetoblack->Checkfadefromblack())
				{
					switch_map = 0;
					fade = false;
					player->gamestate = INGAME;
				}
			}

			if (switch_menu)
			{
				SwitchMenu(!inventory);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		{
			player->position.x -= 50;
			App->render->camera.x += 100;
		}
		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		{
			player->position.x += 50;
			App->render->camera.x -= 100;
		}
		
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	//Draw a map
	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{

	}


	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::AssignValues(Image* assigner, uint var)
{
	int number = var % 10, i= assigner->elements.size()-1;
	assigner->elements[i--]->AssignNumber(number);
	number = var / 10;
	number %= 10;
	assigner->elements[i--]->AssignNumber(number);
	if (assigner->elements.size() > 2)
	{
		number = var/ 100;
		assigner->elements[i]->AssignNumber(number);
	}
}

void j1Scene::LoadUi()
{
	//UI
	hud = App->gui->CreateZeldaMenu();
	Image* Sprite = App->gui->CreateImage({ 18,44,42,16 }, { 12,35 }, "charge");
	force = App->gui->CreateImage({ 21,61,34,10 }, { 4,3 }, "force");
	Sprite->elements.push_back(force);
	hud->AddElement(Sprite);//[0] adding charge bar
	Sprite = App->gui->CreateImage({ 37,20,22,22 }, { 22,12 }, "item");
	Sprite->elements.push_back(App->gui->CreateImage({ 279,257,16,16 }, { 3,3 }, "item_picked"));
	hud->AddElement(Sprite);
	gems = App->gui->CreateImage({ 72,15,8,8 }, { 72,15 }, "gems"); //Gems and value
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { -7,10 }));
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 1,10 }));
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 9,10 }));
	hud->AddElement(gems); 
	//adding gems [2]
	bombs= App->gui->CreateImage({ 100,15,8,8 }, { 100,15 }, "bombs");
	bombs->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { -3,9 }));
	bombs->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 5,9 }));
	hud->AddElement(bombs);
	// adding bombs
	arrows= App->gui->CreateImage({ 121,15,14,8 }, { 121,15 },"arrows");
	arrows->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { -3,9 }));
	arrows->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 5,9 }));
	hud->AddElement(arrows);
	//adding arrows
	hp = App->gui->CreateImage({ 178,15,44,7 }, { 178,15 }, "life");
	hud->AddElement(hp);
	hp->elements.push_back(App->gui->CreateImage({ 177,24,7,7 }, { -21,9 }, "hp1"));
	hp->elements.push_back(App->gui->CreateImage({ 177,24,7,7 }, { -12,9 }, "hp2")); //heart containers
	hp->elements.push_back(App->gui->CreateImage({ 177,24,7,7 }, { -3,9 }, "hp3"));
	
	hud->position = { 0,0 };
	hud->identifier = "hud";
	//Start Menu
	start_menu = App->gui->CreateZeldaMenu();
	Sprite = App->gui->CreateImage({ 1,255,256,224 }, { 0,-224 }, "bg");
	//Button* hotfix = App->gui->CreateButton({ 271,268,32,32 }, { 24,21 - 224 }, { 304,268 }, { 337,268 }, false, "bow");
	//hotfix->selected = true;
	//start_menu->AddElement(hotfix);
	start_menu->Hitbox = Sprite->Hitbox;
	start_menu->AddElement(Sprite);

	start_menu->AddElement(App->gui->CreateImage({ 370,372,32,32 }, { 18,154 - 224 }, "item_info"));
	start_menu->AddElement(App->gui->CreateText(PIXEL, "SELECT AN ITEM", 100, { 19,193 - 224 }, 10));
	start_menu->AddElement(App->gui->CreateImage({ 279,256,16,16 }, { 200,23 - 224 }, "item_picked"));
	start_menu->AddElement(App->gui->CreateText(PIXEL, "   PICK ITEM", 100, { 184,44 - 224 }, 10));
	start_menu->AddElement(App->gui->CreateImage({ 450,273,64,48 }, { 176,77 - 224 }, "pendants"));
	Text* line = App->gui->CreateText(PIXELMORE, "The great warriors charge the bow to shoot further.", 22, { 59,155 - 224 }, 20);
	start_menu->AddElement(line);
	line->Visible(false);
	line = App->gui->CreateText(PIXELMORE, "The more power you apply to it, more distance you will reach.", 22, { 59,155 - 224 }, 20);
	line->Visible(false);
	start_menu->AddElement(line);
	line = App->gui->CreateText(PIXELMORE, "BOOM!!! Explosion!!!", 22, { 59,155 - 224 }, 20);
	line->Visible(false);
	start_menu->AddElement(line);
	start_menu->position = { 0,-224 };
	start_menu->OpenClose(false);
	start_menu->identifier = "start_menu";

	float win_marge = (App->win->GetWidth() - start_menu->Hitbox.w*App->win->GetScale()) / 4;
	hud->Move(true, win_marge);
	start_menu->Move(true, win_marge);
	//pokecombat
	//pokecombat = App->gui->CreatePokemonCombatHud(450,100,300);
}

void j1Scene::SwitchMenu(bool direction)//true for down, false for up
{
	if (direction)
	{
		if (start_menu->position.y < 0)
		{
			start_menu->OpenClose(true);
			start_menu->Move(false, 6.0);
			hud->Move(false, 6.0);
		}
		else
		{
			hud->OpenClose(false);
			switch_menu = false;
			inventory = true;
			player->gamestate = INMENU;
		}
	}
	else
	{
		if (hud->position.y > 0)
		{
			hud->OpenClose(true);
			start_menu->Move(false, -6.0);
			hud->Move(false, -6.0);
		}
		else
		{
			start_menu->OpenClose(false);
			switch_menu = false;
			inventory = false;
			player->gamestate = INGAME;
		}
	}

}

bool j1Scene::Load_new_map(int n)
{
	if (player == NULL)
	{
		player = App->entity_elements->CreatePlayer();
	}

	/*//SET WEAPONS WHEN MAP CHANGES
	if (weapon_equiped == BOMB)
	{
		player->bombmanager = App->entity_elements->CreateBombContainer();
		player->equiped_item = (Weapon*)player->bombmanager;
	}
	if (weapon_equiped == HOOKSHOT)
	{
		player->hook = App->entity_elements->CreateHookshot();
		player->equiped_item = (Weapon*)player->hook;
	}*/

	bool stop_rearch = false;

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);

	for (pugi::xml_node temp = config.child("maps").child("map"); stop_rearch == false; temp = temp.next_sibling())
	{
		if (temp.attribute("n").as_int(0) == n)
		{
			if (n == 1 && switch_map == 0 || switch_map == 6)
			{
				//player position
				player->position.x = temp.child("Link").attribute("pos_x").as_int(0);
				player->position.y = temp.child("Link").attribute("pos_y").as_int(0);
			}
			else
			{
				player->position.x = newPosition.x;
				player->position.y = newPosition.y;
			}



			//Enemies
			pugi::xml_node temp_enemy = temp.child("enemies").child("enemy");
			for (int i = 0; i < temp.child("enemies").attribute("num").as_int(0); i++)
			{
				enemy.push_back(App->entity_elements->CreateSoldier(temp_enemy.attribute("id").as_int(0), temp_enemy));
				temp_enemy = temp_enemy.next_sibling();
			}

			//map
			std::string name_map = temp.attribute("file").as_string("");
			App->map->Load(name_map.c_str(), n);

			//Trainers
			if (temp.child("trainer"))
			{
				poketrainer.push_back(App->entity_elements->CreateTrainer(temp.child("trainer"), 1));
			}

			//Pokemons
			pugi::xml_node temp_pokemon = temp.child("pokemons").child("pokemon");
			for (int i = 0; i < temp.child("pokemons").attribute("num").as_int(0); i++)
			{
				bool mode_stone = temp_pokemon.attribute("mode_stone").as_bool(false);
				if (temp_pokemon.attribute("id").as_int(0) == 2 && mode_stone == false)
				{
					pokemons.push_back(App->entity_elements->CreatePokemon(temp_pokemon, temp_pokemon.attribute("id").as_int(0)));
				}
				else 
				{
					pokemons.push_back(App->entity_elements->CreatePokemon(temp_pokemon, temp_pokemon.attribute("id").as_int(0)));
				}
				temp_pokemon = temp_pokemon.next_sibling();
			}


			//Camera position
			int scale = App->win->GetScale();
			player->camera_follow = temp.child("camera").attribute("follow").as_bool();
			if (player->camera_follow == true)
			{
				int h = App->win->GetHeight() / scale;
				int w = App->win->GetWidth() / scale;
				App->render->camera.x = -((player->position.x - (w / scale)) * scale);
				App->render->camera.y = -((player->position.y - (h / scale)) * scale);

				iPoint size_map = App->map->MapToWorld(App->map->data.width, App->map->data.height);
				if (-App->render->camera.x < 0)
				{
					App->render->camera.x = 0;
				}
				if (-App->render->camera.y < 0)
				{
					App->render->camera.y = 0;
				}
				if (((-App->render->camera.x / scale) + App->win->GetWidth() / scale) > size_map.x)
				{
					App->render->camera.x = (-size_map.x + App->win->GetWidth() / scale) * scale;
				}
				if (((-App->render->camera.y / scale) + App->win->GetHeight() / scale) > size_map.y)
				{
					App->render->camera.y = (-size_map.y + App->win->GetHeight() / scale) * scale;
				}
			}
			else
			{
				iPoint size_pos = App->map->MapToWorld(App->map->data.width, App->map->data.height);
				App->render->camera.x = (App->win->GetWidth() / scale - size_pos.x);
				App->render->camera.y = (App->win->GetHeight() / scale - size_pos.y);
			}
			stop_rearch = true;
		}
	}
	return true;
}


bool j1Scene::Load_Combat_map(int n)
{
	bool stop_rearch = false;

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	start_menu->OpenClose(false);
	hud->OpenClose(false);

	float win_marge = (App->win->GetWidth() - start_menu->Hitbox.w*App->win->GetScale()) / 4;

	for (pugi::xml_node temp = config.child("map_combat").child("map"); stop_rearch == false; temp = temp.next_sibling())
	{
		if (temp.attribute("n").as_int(0) == n)
		{
			//Pokemon Link
			if (n == 7)
			{
				player->pokedex.push_back(App->combatmanager->CreatePokemon(temp.child("Link").child("pokemon"), 1));
				player->pokedex.push_back(App->combatmanager->CreatePokemon(temp.child("Link").child("pokemon").next_sibling(), 2));
				player->pokedex.push_back(App->combatmanager->CreatePokemon(temp.child("Link").child("pokemon").next_sibling().next_sibling(), 3));
			}

			//trainer
			poketrainer.push_back(App->combatmanager->CreateTrainer(temp.child("trainer"), 1));

			pokecombat = App->gui->CreatePokemonCombatHud(player->pokedex.begin()++._Ptr->_Myval, poketrainer.begin()._Ptr->_Myval->GetPokemon(2));
			pokecombat->Move(true, win_marge);
			//map
			std::string name_map = temp.attribute("file").as_string("");
			App->map->Load(name_map.c_str(), n);

			//Load UI (?)
			stop_rearch = true;
		}
	}
	return true;
}


int j1Scene::IdMap()
{
	return id_map;

}

// ---------------------------------------------
pugi::xml_node j1Scene::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = App->fs->Load("Levels.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("levels");

	return ret;
}