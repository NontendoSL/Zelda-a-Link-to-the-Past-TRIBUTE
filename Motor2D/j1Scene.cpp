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
#include "j1SceneIntro.h"

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
		if (first_loop)
		{
			LoadUi();
			
			//App->audio->PlayMusic("audio/music/ZELDA/Zeldakakariko_village.ogg");
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
			App->audio->LoadFx("audio/fx/LTTP_BombBreaksWall.wav");//16 Arrow charge GOOD
			App->audio->LoadFx("audio/fx/LTTP_ArrowHitWall.wav");//17 Arrow hit
			App->audio->LoadFx("audio/fx/LTTP_Arrow.wav");//18
			//Inicialitzate All teleports
			CreateTeleports();
			first_loop = false;
		}
		Load_new_map(1, false);
		help_timer = SDL_GetTicks();
		App->gui->SetGui(ZELDA_HUD);
		start_menu->ResetInventory();
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

			player->ShowHearts();

			if (help_timer + 5000 < SDL_GetTicks() && help_bool)
			{
				player->dialog = App->gui->CreateDialogue("Save me...... Save me...... Link in need your help!, Ganon kidnapped me in the hyrule castle!");
				help_bool = false;
			}

			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
			{
				if (player->dialog != nullptr)
				{
					player->dialog->PushLine(true);
				}
			}
			
			// Change Volume Music -------------------------------------
			if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
			{
				if(volume < 120)
					volume += 10;
				App->audio->VolumeMusic(volume);
			}
			if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
			{
				if (volume > 0)
					volume -= 10;
				App->audio->VolumeMusic(volume);
			}
			// --------------------------------------------------------

			// TP LEVEL -----------------------------------------
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 13;
			}

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 2;
			}

			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 3;
			}

			if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 4;
			}

			if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 5;
			}
			// --------------------------------------------------------

			if (switch_map > 0)
			{
				SwitchMap(useTP);
			}

			if (switch_menu)
			{
				SwitchMenu(!inventory);
			}

			//MINI TP -----------------------------------------------
			if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
			{
				App->scene->player->position.x += 50;
				App->render->camera.x -= 100;
			}
			if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
			{
				App->scene->player->position.y += 50;
				App->render->camera.y -= 100;
			}
			if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				App->scene->player->position.x -= 50;
				App->render->camera.x += 100;
			}
			if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
			{
				App->scene->player->position.y -= 50;
				App->render->camera.y += 100;
			}
			if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			{
				//	App->scene->player->position.x = App->input->GetMousePosition().x;
				//	App->scene->player->position.y = App->input->GetMousePosition().y;
			}
			if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
			{
				App->gui->SetGui(POKEMON_HUD);
			}
			//-------------------------------------------------------
			// TP LEVEL
		}
		
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	//Draw a map
	App->map->Draw(false);

	return update_return;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	// Remove all teleports
	teleports.clear();

	return true;
}

void j1Scene::OnGui(j1GuiEntity* element, GuiAction event)
{
	if (element->identifier == "bow")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			start_menu->Equip("bow");
		}
	}
	if (element->identifier == "hookshot")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			start_menu->Equip("hookshot");
		}
	}
	if (element->identifier == "bomb")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			start_menu->Equip("bomb");
		}
	}
	if (element->identifier == "save_opt")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
		}
	}
	if (element->identifier == "menu_opt")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			start_menu->on_options = false;
			GoMainMenu();
		}
	}
	if (element->identifier == "quit_opt")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			update_return = false;
		}
	}
	if (element->identifier == "pk_bar_hud_1")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
		}
	}
	if (element->identifier == "pk_bar_hud_2")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
		}
	}
	if (element->identifier == "pk_bar_hud_3")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
		}
	}
}

void j1Scene::CreateTeleports()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);
	//Create DirectionMap
	for (pugi::xml_node temp = config.child("teleport").child("map"); temp != NULL; temp = temp.next_sibling())
	{
		for (pugi::xml_node port = temp.child("port"); port != NULL; port = port.next_sibling())
		{
			Teleport teleport;
			teleport.name = port.attribute("name").as_string("");
			teleport.rect.x = port.attribute("rect_x").as_int(0);
			teleport.rect.y = port.attribute("rect_y").as_int(0);
			teleport.id_map = port.attribute("switch_map").as_int(0);
			teleport.position = iPoint(port.attribute("pos_x").as_int(0), port.attribute("pos_y").as_int(0));
			teleports.push_back(teleport);
		}
	}
}

void j1Scene::LoadUi()
{
	//UI
	hud = App->gui->CreateZeldaHud();
	start_menu = App->gui->CreateZeldaMenu();
	poke_hud = App->gui->CreatePokemonWorldHud();
	float win_marge = (App->win->GetWidth() - App->gui->GetEntity("bg")->Hitbox.w*App->win->GetScale()) / 4;
	hud->Move(true, win_marge);
	start_menu->Move(true, win_marge);
	App->gui->SetGui(ZELDA_HUD);
}

void j1Scene::SwitchMenu(bool direction)//true for down, false for up
{
	if (direction)
	{
		if (start_menu->position.y < 0)
		{
			if (gamestate != INMENU)
			{
				gamestate = INMENU;
				App->audio->PlayFx(3);
			}
			start_menu->Move(false, 6.0);
			hud->Move(false, 6.0, true);
		}
		else
		{
			start_menu->position.y = 0;
			switch_menu = false;
			inventory = true;
			App->gui->SetGui(ZELDA_MENU);
			start_menu->ShowItemInfo();
		}
	}
	else
	{
		if (hud->position.y > 0)
		{
			if (start_menu->position.y == 0)
			{
				App->audio->PlayFx(2);
			}
			start_menu->Move(false, -6.0);
			hud->Move(false, -6.0, true);
			start_menu->active = false;
		}
		else
		{
			switch_menu = false;
			inventory = false;
			gamestate = INGAME;
			App->gui->SetGui(ZELDA_HUD);
			start_menu->on_options = false;
		}
	}

}

void j1Scene::GoMainMenu()
{
	if (App->map->CleanUp())
	{
		App->collision->EreseAllColiderPlayer();
		if (player->equiped_item != nullptr)
		{
			weapon_equiped = player->equiped_item->Wtype;
		}
		App->entity_elements->DelteElements();
		App->combatmanager->DeleteElements_combat();

		if (poketrainer != nullptr)
		{
			PokemonCombat* poke = poketrainer->GetPokemon();
			delete poke;
			poke = nullptr;
		}

		if (player->pokedex.size() > 0)
		{
			player->pokedex.clear();
		}
	}
	
	if (inventory)
	{
		start_menu->Move(false, -App->gui->GetEntity("bg")->Hitbox.h);
		hud->Move(false, -App->gui->GetEntity("bg")->Hitbox.h);
		gamestate = INGAME;
	}
	if (player->dialog != nullptr)
	{
		player->dialog->Clear();
	}
	start_menu->active = false;
	App->entity_elements->DeletePlayer(player);
	ingame = false;
	App->intro->fade = true;
	App->gui->SetGui(MAIN_MENU);
}

void j1Scene::ChangeState(GameState state)
{
	if (state >= INGAME && state <= GAMEOVER)
	{
		gamestate = state;
	}
}

void j1Scene::SwitchMap(bool isTP)
{
	if (fade == false)
	{
		App->fadetoblack->FadeToBlack();
		gamestate = INMENU;
		player->SetState(L_IDLE);
		player->SetAnimState(L_IDLE);
		fade = true;
		now_switch = true;
	}

	if (App->fadetoblack->Checkfadetoblack() && now_switch)
	{
		if (switch_map >= FIRST_LEVEL_COMBAT) //id 17 = First level combat map
		{
			combat = true;
		}
		now_switch = false;
		if (App->map->CleanUp())
		{
			App->collision->EreseAllColiderPlayer();
			if (player->equiped_item != nullptr)
			{
				weapon_equiped = player->equiped_item->Wtype;
			}
			App->entity_elements->DelteElements();
			App->combatmanager->DeleteElements_combat();

			if (poketrainer != nullptr)
			{
				PokemonCombat* poke = poketrainer->GetPokemon();
				delete poke;
				poke = nullptr;
			}
			if (player->pokedex.size() > 0) //TODO ELLIOT No with this method
			{
				player->pokedex.clear();
			}

			if (switch_map < FIRST_LEVEL_COMBAT) //id 13 is the first combat map
			{
				combat = false;
				Load_new_map(switch_map, isTP);
			}
			else
			{
				Load_Combat_map(switch_map);
			}
			if (switch_map == 4 && notrepeatmusic)
			{
				notrepeatmusic = false;
				//App->audio->PlayMusic("audio/music/ZELDA/ZeldaHyruleCastle.ogg");
			}
		}
	}
	if (App->fadetoblack->Checkfadefromblack())
	{
		switch_map = 0;
		fade = false;
		gamestate = INGAME;
		id_map = switch_map;
		useTP = false;
	}
}

bool j1Scene::Load_new_map(int n, bool isTP)
{
	if (player == NULL)
	{
		player = App->entity_elements->CreatePlayer();
	}
	if (n == 1)
	{
		player->score = 0;
		player->gems = 0;
		//hud->OpenClose(true);
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
			if (n == 1 && switch_map == 0 || switch_map == 7 || switch_map == 8 || pokecombat != nullptr && n==1 || isTP)
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

			//soldier
			pugi::xml_node temp_enemy = temp.child("soldier").child("soldier");
			for (int i = 0; i < temp.child("soldier").attribute("num").as_int(0); i++)
			{
				App->entity_elements->CreateSoldier(temp_enemy.attribute("id").as_int(0), temp_enemy);
				temp_enemy = temp_enemy.next_sibling();
			}

			//map
			std::string name_map = temp.attribute("file").as_string("");
			App->map->Load(name_map.c_str(), n);

			//Trainers
			if (temp.child("trainer"))
			{
				poketrainer = App->entity_elements->CreateTrainer(temp.child("trainer"), 1);
			}

			//items
			pugi::xml_node temp_item = temp.child("items").child("item");
			for (int i = 0; i < temp.child("items").attribute("num").as_int(0); i++)
			{
				App->entity_elements->CreateItem(temp_item.attribute("id").as_int(0), iPoint(temp_item.attribute("pos_x").as_int(0), temp_item.attribute("pos_y").as_int(0)));
				temp_item = temp_item.next_sibling();
			}

			//Pokemons
			pugi::xml_node temp_pokemon = temp.child("pokemons").child("pokemon");
			for (int i = 0; i < temp.child("pokemons").attribute("num").as_int(0); i++)
			{
				bool mode_stone = temp_pokemon.attribute("mode_stone").as_bool(false);
				if (temp_pokemon.attribute("id").as_int(0) == 2 && mode_stone == false)
				{
					App->entity_elements->CreatePokemon(temp_pokemon, temp_pokemon.attribute("id").as_int(0));
				}
				else 
				{
					App->entity_elements->CreatePokemon(temp_pokemon, temp_pokemon.attribute("id").as_int(0));
				}
				temp_pokemon = temp_pokemon.next_sibling();
			}

			//BCTrooper
			if (temp.child("BCTrooper"))
			{
				pugi::xml_node temp_bctrooper = temp.child("BCTrooper");
				App->entity_elements->CreateBCTrooper(temp_bctrooper);
			}

			//Vilagers
			for (pugi::xml_node vilager = temp.child("vilagers").child("vilager"); vilager != NULL; vilager = vilager.next_sibling())
			{
				App->entity_elements->CreateVilager(vilager);
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


	//start_menu->OpenClose(false);
	//hud->OpenClose(false);

	float win_marge = (App->win->GetWidth() - App->gui->GetEntity("bg")->Hitbox.w*App->win->GetScale()) / 4;

	for (pugi::xml_node temp = config.child("map_combat").child("map"); stop_rearch == false; temp = temp.next_sibling())
	{
		if (temp.attribute("n").as_int(0) == n)
		{
			//Pokemon Link
			if (n == 13)
			{
				player->pokedex.push_back(App->combatmanager->CreatePokemon(temp.child("Link").child("pokemon"), 1));
				player->pokedex.push_back(App->combatmanager->CreatePokemon(temp.child("Link").child("pokemon").next_sibling(), 2));
				player->pokedex.push_back(App->combatmanager->CreatePokemon(temp.child("Link").child("pokemon").next_sibling().next_sibling(), 3));
			}

			//trainer
			poketrainer = App->combatmanager->CreateTrainer(temp.child("trainer"), 1);

			if (pokecombat == nullptr)
			{
				pokecombat = App->gui->CreatePokemonCombatHud(player->pokedex.begin()._Ptr->_Myval, poketrainer->GetPokemon()); //TODO ELliot no need GetPokemon()
				pokecombat->Move(true, win_marge);
			}
			else
			{
				//pokecombat->OpenClose(true);
				pokecombat->CombatInfo(player->pokedex.begin()._Ptr->_Myval, poketrainer->GetPokemon());
			}

			//map
			std::string name_map = temp.attribute("file").as_string("");
			App->map->Load(name_map.c_str(), n);
			//Edit
			App->map->EditCost(18, 17, 0);
			App->map->EditCost(19, 17, 0);
			App->map->EditCost(18, 18, 0);
			App->map->EditCost(19, 18, 0);

			//Load UI (?)
			stop_rearch = true;
		}
	}
	App->gui->SetGui(POKEMON_COMBAT);
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