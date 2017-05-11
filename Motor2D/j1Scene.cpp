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
#include "ParticleManager.h"
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
			
			App->audio->PlayMusic("audio/music/ZELDA/Zeldakakariko_village.ogg");
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
			App->audio->LoadFx("audio/fx/LTTP_Stun.wav");//19
			App->audio->LoadFx("audio/fx/LTTP_ItemGet.wav");//20

			//Init All teleports
			CreateTeleports();
			first_loop = false;
		}
		Load_new_map(1, true);
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

			/*if (help_timer + 2000 < SDL_GetTicks() && help_bool)
			{
				player->dialog = App->gui->CreateDialogue("Link... I need your help. Head to the castle and you'll find guidance. Hurry up!");
				help_bool = false;
			}*/

			//Make advance the dialogue text.
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_DOWN)
			{
				if (player->dialog != nullptr)
				{
					player->dialog->PushLine(true);
				}
			}
			
			if (/*goPokemon && */player->dialog != nullptr)
			{
				dialog_inmapZelda = true;
			}

			if (dialog_inmapZelda && player->dialog == nullptr)
			{
				if (combat_map_id != 0)
				{
					switch_map = combat_map_id;
					combat_map_id = 0;
					useTP = true;
					dialog_inmapZelda = false;
					goPokemon = false;
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
				switch_map = 10;
			}

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 18;
			}

			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 19;
			}

			if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 20;
			}

			if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 21;
			}

			/*if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 20;
			}*/

			if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 16;
			}

			/*if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
			{
				useTP = true;
				switch_map = 21;
			}*/
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
					//App->scene->player->position.x = App->input->GetMousePosition().x;
					//App->scene->player->position.y = App->input->GetMousePosition().y;
			}
			if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
			{
				App->gui->SetGui(POKEMON_SHOP);
			}
			if (App->input->GetKey(SDL_SCANCODE_0) == KEY_UP)
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
	if (element->identifier == "pk_menu:pokemon")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			//poke_menu->active = false;
			poke_menu->MoveIn(false);
			App->gui->SetGui(POKEMON_HUD);
			poke_hud->active = true;
			App->gui->SetFocus(poke_hud->GetFirst());
		}
	}
	if (element->identifier == "pk_menu:bag")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			//poke_menu->MoveIn(false);
			App->gui->SetGui(POKEMON_BAG);
		}
	}
	if (element->identifier == "pk_menu:link")
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
	if (element->identifier == "pk_menu:menu")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			poke_menu->MoveIn(false);
			GoMainMenu();
		}
	}
	if (element->identifier == "pk_menu:save")
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
	if (element->identifier == "pk_menu:options")
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
	if (element->identifier == "pk_menu:exit")
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
	if (element->identifier == "pk_bag:CLOSE")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			App->gui->SetGui(POKEMON_MENU);
		}
	}
	if (element->identifier == "pk_bag:BLAZIKEN")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			poke_bag->selecting_poke = false;
			poke_bag->AddItem(poke_bag->selected_name.c_str(), false);
			App->gui->SetFocus(poke_bag->GetFirst());
			poke_bag->ShowItemInfo();
		}
	}
	if (element->identifier == "pk_bag:SWAMPERT")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			poke_bag->selecting_poke = false;
			poke_bag->AddItem(poke_bag->selected_name.c_str(), false);
			App->gui->SetFocus(poke_bag->GetFirst());
			poke_bag->ShowItemInfo();
		}
	}
	if (element->identifier == "pk_bag:SCEPTILE")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			poke_bag->selecting_poke = false;
			poke_bag->AddItem(poke_bag->selected_name.c_str(), false);
			App->gui->SetFocus(poke_bag->GetFirst());
			poke_bag->ShowItemInfo();
		}
	}
	if (element->identifier == "pcshop:BUTTON1")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			if (player->pokecash >= 30)
			{
				player->pokecash -= 30;
				poke_hud->RefreshMoney();
				if (poke_bag->AddItem("pk_bag:DEF PROTEIN", true))
				{
					poke_shop->PopText(" PROTEIN PURCHASED");
				}
				else
				{
					poke_shop->PopText("MAX PROTEINS REACHED");
				}
			}
			else
			{
				poke_shop->PopText("NOT ENOUGH POKECASH");
			}
		}
	}
	if (element->identifier == "pcshop:BUTTON2")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			if (player->pokecash >= 40)
			{
				player->pokecash -= 40;
				poke_hud->RefreshMoney();
				if (poke_bag->AddItem("pk_bag:HP UP", true))
				{
					poke_shop->PopText("   HP UP PURCHASED");
				}
				else
				{
					poke_shop->PopText("  MAXHP UP REACHED");
				}
			}
			else
			{
				poke_shop->PopText("NOT ENOUGH POKECASH");
			}
		}
	}
	if (element->identifier == "pcshop:BUTTON3")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			if (player->pokecash >= 50)
			{
				player->pokecash -= 50;
				poke_hud->RefreshMoney();
				if (poke_bag->AddItem("pk_bag:X ATTACK", true))
				{
					poke_shop->PopText(" X ATTACK PURCHASED");
				}
				else
				{
					poke_shop->PopText("MAX X ATTACK REACHED");
				}
			}
			else
			{
				poke_shop->PopText("NOT ENOUGH POKECASH");
			}
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
	poke_menu = App->gui->CreatePokemonWorldMenu();
	poke_bag = App->gui->CreatePokemonWorldBag();
	poke_shop = App->gui->CreatePokemonWorldShop();
	float win_marge = (App->win->GetWidth() - App->gui->GetEntity("pokemon bag")->Hitbox.w*App->win->GetScale()) / 4;
	App->gui->GetEntity("pokemon bag")->position.x += win_marge;
	float h_marge = (App->win->GetHeight() - App->gui->GetEntity("pokemon bag")->Hitbox.h*App->win->GetScale()) / 4;
	App->gui->GetEntity("pokemon bag")->position.y += h_marge;
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

		if (poke_enemy != nullptr)
		{
			poke_enemy = nullptr;
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
		now_switch = false;
		if (App->map->CleanUp())
		{
			App->particlemanager->DeleteAllGroups();
			App->collision->EreseAllColiderPlayer();
			if (player->equiped_item != nullptr)
			{
				weapon_equiped = player->equiped_item->Wtype;
			}
			App->entity_elements->DelteElements();
			if (App->combatmanager->Getsize_elements() > 0)
			{
				App->combatmanager->DeleteElements_combat();
			}

			if (poke_enemy != nullptr)
			{
				poke_enemy = nullptr;
			}

			if (switch_map < FIRST_LEVEL_COMBAT) //id 17 is the first combat map
			{
				combat = false;
				Load_new_map(switch_map, isTP);
			}
			else
			{
				combat = true;
				Load_Combat_map(switch_map);
			}
			if (switch_map == 4 && notrepeatmusic)
			{
				notrepeatmusic = false;
				App->audio->PlayMusic("audio/music/ZELDA/ZeldaHyruleCastle.ogg");
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

	if (n == 9)
	{
		App->gui->SetGui(POKEMON_HUD);
		player->pokecash = player->gems * 3;
		poke_hud->RefreshMoney();
	}

	if (n == 5)
	{
		goPokemon = true;
	}

	bool stop_rearch = false;

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);

	for (pugi::xml_node temp = config.child("maps").child("map"); stop_rearch == false; temp = temp.next_sibling())
	{
		if (temp.attribute("n").as_int(0) == n)
		{
			if (isTP)
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

			//items
			pugi::xml_node temp_item = temp.child("items").child("item");
			for (int i = 0; i < temp.child("items").attribute("num").as_int(0); i++)
			{
				App->entity_elements->CreateItem(temp_item.attribute("id").as_int(0), iPoint(temp_item.attribute("pos_x").as_int(0), temp_item.attribute("pos_y").as_int(0)));
				temp_item = temp_item.next_sibling();
			}

			//DynObjects - ¡¡ONLY SING!!
			for (pugi::xml_node dynobject = temp.child("dynobjects").child("dynobject"); dynobject != NULL; dynobject = dynobject.next_sibling())
			{
				if (dynobject.attribute("id").as_int(1) == 1)
				{
					App->entity_elements->CreateDynObject(iPoint(dynobject.attribute("pos_x").as_int(0), dynobject.attribute("pos_y").as_int(0)), dynobject.attribute("id").as_int(1), n, true, dynobject);
				}
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

			if (temp.child("Ganon"))
			{
				pugi::xml_node temp_Ganon = temp.child("Ganon");
				App->entity_elements->CreateGanon(temp_Ganon);
			}

			//Villagers
			for (pugi::xml_node vilager = temp.child("vilagers").child("vilager"); vilager != NULL; vilager = vilager.next_sibling())
			{
				App->entity_elements->CreateVillager(vilager);
			}

			//EditCost (Only in EliteFour)
			if (player->state_complet == true)
			{
				App->gui->SetGui(ZELDA_HUD);
				for (pugi::xml_node editcost = temp.child("editcost").child("edit"); editcost != NULL; editcost = editcost.next_sibling())
				{
					App->map->EditCost(editcost.attribute("pos_x").as_int(0), editcost.attribute("pos_y").as_int(0), 0);
				}
				player->state_complet = false;
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
	last_map = n;
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
	if (player->pokedex.size() == 0)
	{
		player->pokedex.push_back(App->combatmanager->CreatePokemon(config.child("map_combat").child("Link").child("pokemon"), 1));
		player->pokedex.push_back(App->combatmanager->CreatePokemon(config.child("map_combat").child("Link").child("pokemon").next_sibling(), 2));
		player->pokedex.push_back(App->combatmanager->CreatePokemon(config.child("map_combat").child("Link").child("pokemon").next_sibling().next_sibling(), 3));
	}

	float win_marge = (App->win->GetWidth() - App->gui->GetEntity("bg")->Hitbox.w*App->win->GetScale()) / 4;

	for (pugi::xml_node temp = config.child("map_combat").child("map"); stop_rearch == false; temp = temp.next_sibling())
	{
		if (temp.attribute("n").as_int(0) == n)
		{
			//trainer
			//poketrainer = App->combatmanager->CreateTrainer(temp.child("trainer"), 1);
			poke_enemy = App->combatmanager->CreatePokemon(temp.child("trainer").child("pokemon"), temp.child("trainer").child("pokemon").attribute("id").as_int(4));

			//idMap
			App->combatmanager->id_map_combat = n;

			//Pokemon Link
			if (App->scene->poke_hud->GetPokeOrder(0) == "pk_bar_blaziken")
			{
				App->combatmanager->PrepareToCombat(player->pokedex.begin()._Ptr->_Myval, n, 1);
			}
			else if (App->scene->poke_hud->GetPokeOrder(0) == "pk_bar_sceptile")
			{
				App->combatmanager->PrepareToCombat(player->pokedex.begin()._Ptr->_Next->_Myval, n, 2);
			}
			else
			{
				App->combatmanager->PrepareToCombat(player->pokedex.begin()._Ptr->_Next->_Next->_Myval, n, 3);
			}

			if (pokecombat == nullptr)
			{
				pokecombat = App->gui->CreatePokemonCombatHud(App->combatmanager->pokemon_active_link, App->combatmanager->pokemon_active_trainer); //TODO ELliot no need GetPokemon()
				pokecombat->Move(true, win_marge);
			}
			else
			{
				//pokecombat->OpenClose(true);
				pokecombat->CombatInfo(App->combatmanager->pokemon_active_link, App->combatmanager->pokemon_active_trainer);
			}

			//map
			std::string name_map = temp.attribute("file").as_string("");
			App->map->Load(name_map.c_str(), n);


			//Edit Cost
			for (pugi::xml_node editcost = temp.child("editcost").child("edit"); editcost != NULL; editcost = editcost.next_sibling())
			{
				App->map->EditCost(editcost.attribute("pos_x").as_int(0), editcost.attribute("pos_y").as_int(0), App->map->data.tilesets[0]->firstgid + 1);
			}

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