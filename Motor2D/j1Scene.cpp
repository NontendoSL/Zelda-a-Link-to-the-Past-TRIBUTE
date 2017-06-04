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
#include "Video.h"
#include "CutsceneManager.h"

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
			App->audio->VolumeMusic(50);
			App->audio->LoadFx("audio/fx/LTTP_Pause_Open.wav"); //1
			App->audio->LoadFx("audio/fx/LTTP_Pause_Close.wav"); //3
			App->audio->LoadFx("audio/fx/LTTP_Rupee1.wav");//4
			App->audio->LoadFx("audio/fx/LTTP_Sword_Attack.wav");//5
			App->audio->LoadFx("audio/fx/LTTP_BombLay.wav");//6
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
			App->audio->LoadFx("audio/fx/PKMN_Paying.wav");//21
			App->audio->LoadFx("audio/fx/PKMN_IncreasingStat.wav");//22
			App->audio->LoadFx("audio/fx/PKMN_Select.wav");//23
			App->audio->LoadFx("audio/fx/PKMN_MenuOpen.wav");//24
			App->audio->LoadFx("audio/fx/LTTP_Melee.wav");//25
			App->audio->LoadFx("audio/fx/LTTP_GanonJump.wav");//26
			App->audio->LoadFx("audio/fx/LTTP_Firbats.wav");//27
			App->audio->LoadFx("audio/fx/LTTP_Teleport.wav");//28
			App->audio->LoadFx("audio/fx/LTTP_Grass_Destroyed.wav");//29
			App->audio->LoadFx("audio/fx/LTTP_Break.wav");//30
			App->audio->LoadFx("audio/fx/PKMN_Ice.wav");//31
			App->audio->LoadFx("audio/fx/PKMN_Dragon.wav");//32
			App->audio->LoadFx("audio/fx/PKMN_Ghost.wav");//33
			App->audio->LoadFx("audio/fx/PKMN_Heal.wav");//34
			App->audio->LoadFx("audio/fx/LTTP_LinkDeath.wav");//35
			App->audio->LoadFx("audio/fx/LTTP_Menu_Cursor.wav");//36
			App->audio->LoadFx("audio/fx/PKMN_Bubble.wav");//37
			App->audio->LoadFx("audio/fx/PKMN_BulletSeed.wav");//38
			//Init All teleports
			CreateTeleports();
			first_loop = false;
		}

		if (new_game == true)
		{
			NewGame();
			App->gui->SetGui(ZELDA_HUD);
			start_menu->ResetInventory();
			help_bool = true;
			new_game = false;
			sleep_start = true;
			playVideo = true;
		}

		else if (continue_game == true)
		{
			App->audio->PlayMusic("audio/music/ZELDA/Zeldakakariko_village.ogg", 0);
			ContinueGame();
			continue_game = false;
		}

		help_timer = SDL_GetTicks();
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
	BROFILER_CATEGORY("Update_Scene", Profiler::Color::DarkGreen);

	if (ingame == true)
	{
		if (sleep_start)
		{
			if (playVideo)
			{
				gamestate = INMENU;
				SDL_Rect r = { 0, 0, 640, 480 };
				App->video->PlayVideo("Sleeping.ogg", r);
				playVideo = false;
			}
			else
			{
				if (App->video->video_finished)
				{
					App->video->ResetValues();
					App->audio->CleanUp();
					App->audio->Awake(pugi::xml_node(nullptr));
					App->audio->Start();
					LoadFX();
					App->audio->PlayMusic("audio/music/ZELDA/Zeldakakariko_village.ogg", 0);
					App->audio->VolumeMusic(50);
					gamestate = INGAME;
					sleep_start = false;
				}
			}
		}
		player->ShowHearts();

		if (help_timer + 2000 < SDL_GetTicks() && help_bool)
		{
			player->dialog = App->gui->CreateDialogue("Link... I need your help. Head to the castle and you'll find guidance. Hurry up!", 0);
			help_bool = false;
		}

		if (gamestate == INMENU && stop == false)
		{
			timepause.Start();
		}
		else if (gamestate == INGAME)
		{
			if (timepause.ReadSec() >= 1.0f)
				stop = false;
		}


		//Make advance the dialogue text.
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_Y) == EVENTSTATE::E_DOWN)
		{
			if (player->dialog != nullptr)
			{
				player->dialog->PushLine(true);
			}
		}

		if (player->dialog != nullptr)
		{
			dialog_inmapZelda = true;
		}

		if (joy_talk && player->dialog == nullptr && last_map == 10 && cash_swapped == false)
		{
			player->pokecash = player->gems * 12;
			if (player->pokecash < 50)
			{
				player->pokecash = 50;
			}
			poke_hud->RefreshMoney();
			joy_talk = false;
			cash_swapped = true;
		}

		if (dialog_inmapZelda && player->dialog == nullptr)
		{
			if (combat_map_id != 0 || cutscene_id != -1 || last_map == 16 && key_boss)
			{
				if (playVideo)
				{
					if (fade == false)
					{
						App->fadetoblack->FadeToBlack();
						gamestate = INMENU;
						fade = true;
						now_switch = true;
					}

					if (App->fadetoblack->Checkfadetoblack() && now_switch)
					{
						SDL_Rect r = { 0, 0, 640, 480 };
						if (key_boss)
						{
							App->entity_elements->DeleteElement("door");
							App->video->PlayVideo("Pendant.ogg", r);
						}
						else
						{
							App->video->PlayVideo("ToPokemonWorld320.ogg", r);
						}
						fade = false;
						playVideo = false;
						waitVideo = true;
					}
				}
				else
				{
					if (App->video->video_finished)
					{
						if (notrepeatCombat)
						{
							if (cutscene_id != -1)
							{
								App->cs_manager->StartCutscene(cutscene_id);
								cutscene_id = -1;
							}
							if (combat_map_id != 0)
							{
								switch_map = combat_map_id;
								combat_map_id = 0;
								useTP = true;
							}
							if (waitVideo)
							{
								App->video->ResetValues();
								App->audio->CleanUp();
								App->audio->Awake(pugi::xml_node(nullptr));
								App->audio->Start();
								LoadFX();
								if (key_boss == false)
								{
									SwitchMap(useTP);
								}
								else
								{
									waitVideo = false;
									gamestate = INGAME;
								}

							}
							dialog_inmapZelda = false;
							goPokemon = false;
							notrepeatCombat = false;
							key_boss = false;
						}
						else
						{
							dialog_inmapZelda = false;
						}
					}
				}
			}
		}

		// Change Volume Music -------------------------------------
		if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		{
			if (volume < 120)
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
			player->sword_equiped = true;
			useTP = true;
			switch_map = 16;
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			useTP = true;
			switch_map = 9;
		}

		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			useTP = true;
			switch_map = 5;
		}

		/*if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		{
			useTP = true;
			switch_map = 20;
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
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			App->scene->player->gems += 1;
		}
		/*if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
				//App->scene->player->position.x = App->input->GetMousePosition().x;
				//App->scene->player->position.y = App->input->GetMousePosition().y;
		}
		if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN && App->gui->GetGuiState()==POKEMON_HUD)
		{
			App->gui->SetGui(POKEMON_SHOP);
		}*/

		//-------------------------------------------------------
		// TP LEVEL

		//WEAPONS TEST ---------
		if (player->setWeapons == false && App->input->GetKey(SDL_SCANCODE_P) &&
			App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
		{
			player->bow = App->entity_elements->CreateBow();
			App->scene->start_menu->PickItem("bow");
			player->arrows = 99;
			player->bombmanager = App->entity_elements->CreateBombContainer();
			App->scene->start_menu->PickItem("bomb");
			player->bombs = 99;
			player->setWeapons = true;
			player->sword_equiped = true;
			player->hook = App->entity_elements->CreateHookshot();
			App->scene->start_menu->PickItem("hookshot");
			player->gems = 999;
		}
		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN &&
			App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
		{
			if (modeGod)
			{
				App->gui->GetEntity("life")->visible = true;
				for (int i = 0; i < App->gui->GetEntity("life")->elements.size(); i++)
				{
					App->gui->GetEntity("life")->elements[i]->visible = true;
				}
				modeGod = false;
			}
			else
			{
				//App->gui->GetEntity("life")->visible = false;
				for (int i = 0; i < App->gui->GetEntity("life")->elements.size(); i++)
				{
					App->gui->GetEntity("life")->elements[i]->visible = false;
				}
				modeGod = true;
			}
		}
		// ---------

		//Set picked object the last 
		if (player->picked_object != nullptr && swap_object == true)
		{
			App->entity_elements->SwapObject(player->picked_object);
			swap_object = false;
		}

		//Blit Ganon Last
		if (swap_ganon == true)
		{
			App->entity_elements->SwapGanon();
			swap_ganon = false;
		}

		//Blit Player Last
		if (swap_player == true)
		{
			App->entity_elements->SwapPlayer();
			swap_player = false;
		}


	}
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	//Draw a map
	if (waitVideo == false)
	{
		App->map->Draw(false);
	}
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
			App->SaveGame("save_file.xml");
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
			App->audio->PlayFx(23);
		}
		else
		{
			((Button*)element)->click = false;
			App->audio->PlayFx(23);
		}
	}
	if (element->identifier == "pk_bar_hud_2")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
			App->audio->PlayFx(23);
		}
		else
		{
			((Button*)element)->click = false;
			App->audio->PlayFx(23);
		}
	}
	if (element->identifier == "pk_bar_hud_3")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
			App->audio->PlayFx(23);
		}
		else
		{
			((Button*)element)->click = false;
			App->audio->PlayFx(23);
		}
	}
	if (element->identifier == "pk_menu:pokemon")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
			App->audio->PlayFx(24);
		}
		else
		{
			((Button*)element)->click = false;
			//poke_menu->active = false;
			poke_menu->MoveIn(false);
			App->gui->SetGui(POKEMON_HUD);
			poke_hud->active = true;
			App->gui->SetFocus(poke_hud->GetFirst());
			gamestate = INMENU;
		}
	}
	if (element->identifier == "pk_menu:bag")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
			App->audio->PlayFx(24);
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
			poke_menu->trainer_card = true;
			App->gui->GetEntity("trainer card")->visible = true;
			App->gui->GetEntity("playtime card")->visible = true;
			App->gui->GetEntity("money card")->visible = true;
			poke_menu->WritePlayTime();
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
			App->SaveGame("save_file.xml");
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
			App->audio->PlayFx(24);
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
			if (App->combatmanager->GiveItem(0, poke_bag->selected_name.c_str()))
			{
				poke_bag->AddItem(poke_bag->selected_name.c_str(), false);
				poke_hud->GiveItem("BLAZIKEN", poke_bag->selected_name.c_str());
				App->audio->PlayFx(22);
				poke_bag->ShowItemInfo();
			}
			else
			{
				poke_bag->PopOut("BLAZIKEN IS ALREADY FULLY EQUIPED");
			}
			poke_bag->selecting_poke = false;
			App->gui->SetFocus(poke_bag->GetFirst());
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
			if (App->combatmanager->GiveItem(2, poke_bag->selected_name.c_str()))
			{
				poke_bag->AddItem(poke_bag->selected_name.c_str(), false);
				poke_hud->GiveItem("SWAMPERT", poke_bag->selected_name.c_str());
				App->audio->PlayFx(22);
				poke_bag->ShowItemInfo();
			}
			else
			{
				poke_bag->PopOut("SWAMPERT IS ALREADY FULLY EQUIPED");
			}
			poke_bag->selecting_poke = false;
			App->gui->SetFocus(poke_bag->GetFirst());
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
			if (App->combatmanager->GiveItem(1, poke_bag->selected_name.c_str()))
			{
				poke_bag->AddItem(poke_bag->selected_name.c_str(), false);
				poke_hud->GiveItem("SCEPTILE", poke_bag->selected_name.c_str());
				App->audio->PlayFx(22);
				poke_bag->ShowItemInfo();
			}
			else
			{
				poke_bag->PopOut("SCEPTILE IS ALREADY FULLY EQUIPED");
			}
			poke_bag->selecting_poke = false;
			App->gui->SetFocus(poke_bag->GetFirst());
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
			if (player->pokecash >= 40)
			{
				if (poke_bag->AddItem("pk_bag:DEF PROTEIN", true))
				{
					poke_shop->PopText(" PROTEIN PURCHASED");
					player->pokecash -= 40;
					poke_hud->RefreshMoney();
					App->audio->PlayFx(21);
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
			if (player->pokecash >= 50)
			{
				if (poke_bag->AddItem("pk_bag:HP UP", true))
				{
					player->pokecash -= 50;
					poke_hud->RefreshMoney();
					poke_shop->PopText("   HP UP PURCHASED");
					App->audio->PlayFx(21);
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
			if (player->pokecash >= 60)
			{
				if (poke_bag->AddItem("pk_bag:X ATTACK", true))
				{
					poke_shop->PopText(" X ATTACK PURCHASED");
					player->pokecash -= 60;
					poke_hud->RefreshMoney();
					App->audio->PlayFx(21);
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
				timepaused32 = SDL_GetTicks();
				stop = true;
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
			timepaused32 = SDL_GetTicks() - timepaused32;
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
	if (fade == false && waitVideo == false)
	{
		App->fadetoblack->FadeToBlack();
		gamestate = INMENU;
		player->SetState(L_IDLE);
		player->SetAnimState(L_IDLE);
		fade = true;
		now_switch = true;
	}
	/*else if(fade == false)
	{
		App->fadetoblack->FadeFromBlack();
		gamestate = INMENU;
		player->SetState(L_IDLE);
		player->SetAnimState(L_IDLE);
		fade = true;
		now_switch = true;
	}*/

	if (App->fadetoblack->Checkfadetoblack() && now_switch || waitVideo && now_switch)
	{
		waitVideo = false;
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
		useTP = false;
	}
}

bool j1Scene::Save(pugi::xml_node& node) const
{
	bool ret = true;

	//Player node
	pugi::xml_node checkpoint = node.append_child("CheckPoint");
	player->SaveData(checkpoint);

	return ret;
}

bool j1Scene::Load(pugi::xml_node& checknode)
{
	bool ret = true;

	pugi::xml_node node = checknode.child("CheckPoint");

	pugi::xml_node curr_node = node.child("HP");
	Check.hp_hearts.x = curr_node.attribute("max").as_int(6);
	Check.hp_hearts.y = curr_node.attribute("current").as_int(6);

	curr_node = node.child("MAP");
	Check.map_id = curr_node.attribute("id").as_int(1);

	curr_node = node.child("RESOURCES");
	Check.rupees = curr_node.attribute("rupees").as_uint(0);
	Check.arrows = curr_node.attribute("arrows").as_uint(0);
	Check.bombs = curr_node.attribute("bombs").as_uint(0);

	curr_node = node.child("WEAPONS");
	Check.bow_picked = curr_node.attribute("bow").as_bool(false);
	Check.bombcontainer_picked = curr_node.attribute("bombmanager").as_bool(false);
	Check.hookshot_picked = curr_node.attribute("hookshot").as_bool(false);
	Check.sword_picked = curr_node.attribute("sword").as_bool(false);
	
	curr_node = node.child("UI");
	Check.world = curr_node.attribute("world").as_string("Zelda");
	
	curr_node = node.child("POKECASH");
	Check.cash = curr_node.attribute("cash").as_uint(0);

	curr_node = node.child("POKEMON");
	Check.blaz_hp = curr_node.child("Blaziken").attribute("hp").as_int(0);
	Check.blaz_atk = curr_node.child("Blaziken").attribute("atk").as_int(0);
	Check.blaz_def = curr_node.child("Blaziken").attribute("def").as_int(0);
	Check.blaz_pos = curr_node.child("Blaziken").attribute("pos").as_int(0);

	Check.scept_hp = curr_node.child("Sceptyle").attribute("hp").as_int(0);
	Check.scept_atk = curr_node.child("Sceptyle").attribute("atk").as_int(0);
	Check.scept_def = curr_node.child("Sceptyle").attribute("def").as_int(0);
	Check.scept_pos = curr_node.child("Sceptyle").attribute("pos").as_int(0);

	Check.swamp_hp = curr_node.child("Swampert").attribute("hp").as_int(0);
	Check.swamp_atk = curr_node.child("Swampert").attribute("atk").as_int(0);
	Check.swamp_def = curr_node.child("Swampert").attribute("def").as_int(0);
	Check.swamp_pos = curr_node.child("Swampert").attribute("pos").as_int(0);

	return ret;
}

void j1Scene::LoadFX()
{
	App->intro->Menu_Cursor = App->audio->LoadFx("audio/fx/LTTP_Menu_Cursor.wav");
	App->audio->LoadFx("audio/fx/LTTP_Pause_Open.wav"); //1
	App->audio->LoadFx("audio/fx/LTTP_Pause_Close.wav"); //3
	App->audio->LoadFx("audio/fx/LTTP_Rupee1.wav");//4
	App->audio->LoadFx("audio/fx/LTTP_Sword_Attack.wav");//5
	App->audio->LoadFx("audio/fx/LTTP_BombLay.wav");//6
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
	App->audio->LoadFx("audio/fx/PKMN_Paying.wav");//21
	App->audio->LoadFx("audio/fx/PKMN_IncreasingStat.wav");//22
	App->audio->LoadFx("audio/fx/PKMN_Select.wav");//23
	App->audio->LoadFx("audio/fx/PKMN_MenuOpen.wav");//24
	App->audio->LoadFx("audio/fx/LTTP_Melee.wav");//25
	App->audio->LoadFx("audio/fx/LTTP_GanonJump.wav");//26
	App->audio->LoadFx("audio/fx/LTTP_Firbats.wav");//27
	App->audio->LoadFx("audio/fx/LTTP_Teleport.wav");//28
	App->audio->LoadFx("audio/fx/LTTP_Grass_Destroyed.wav");//29
	App->audio->LoadFx("audio/fx/LTTP_Break.wav");//30
	App->audio->LoadFx("audio/fx/PKMN_Ice.wav");//31
	App->audio->LoadFx("audio/fx/PKMN_Dragon.wav");//32
	App->audio->LoadFx("audio/fx/PKMN_Ghost.wav");//33
	App->audio->LoadFx("audio/fx/PKMN_Heal.wav");//34
	App->audio->LoadFx("audio/fx/LTTP_LinkDeath.wav");//35
	App->audio->LoadFx("audio/fx/LTTP_Menu_Cursor.wav");//36
}

void j1Scene::SetItemsinUI()
{
	/*BLAZIKEN*/
	for (uint i = 0; i < Check.blaz_hp; i++)
	{
		poke_hud->LoadItem("BLAZIKEN","pk_bag:HP UP");
	}
	for (uint i = 0; i < Check.blaz_def; i++)
	{
		poke_hud->LoadItem("BLAZIKEN", "pk_bag:DEF PROTEIN");
	}
	for (uint i = 0; i < Check.blaz_atk; i++)
	{
		poke_hud->LoadItem("BLAZIKEN", "pk_bag:X ATTACK");
	}

	/*SCEPTYLE*/
	for (uint i = 0; i < Check.scept_hp; i++)
	{
		poke_hud->LoadItem("SCEPTILE", "pk_bag:HP UP");
	}
	for (uint i = 0; i < Check.scept_def; i++)
	{
		poke_hud->LoadItem("SCEPTILE", "pk_bag:DEF PROTEIN");
	}
	for (uint i = 0; i < Check.scept_atk; i++)
	{
		poke_hud->LoadItem("SCEPTILE", "pk_bag:X ATTACK");
	}


	/*SWAMPERT*/
	for (uint i = 0; i < Check.swamp_hp; i++)
	{
		poke_hud->LoadItem("SWAMPERT", "pk_bag:HP UP");
	}
	for (uint i = 0; i < Check.swamp_def; i++)
	{
		poke_hud->LoadItem("SWAMPERT", "pk_bag:DEF PROTEIN");
	}
	for (uint i = 0; i < Check.swamp_atk; i++)
	{
		poke_hud->LoadItem("SWAMPERT", "pk_bag:X ATTACK");
	}


}

bool j1Scene::NewGame()
{
	if (player == NULL)
	{
		player = App->entity_elements->CreatePlayer();
	}

	Load_new_map(1, true);

	return true;
}

bool j1Scene::ContinueGame()
{
	if (player == NULL)
	{
		player = App->entity_elements->CreatePlayer();
	}

	player->LoadStats();

	//SET ZELDA UI ---------------------------------------------------------
	if (Check.world == "Zelda")
	{
		App->gui->SetGui(ZELDA_HUD);
		if (player->bow != nullptr)
		{
			App->scene->start_menu->PickItem("bow");
		}
		if (player->hook != nullptr)
		{
			App->scene->start_menu->PickItem("hookshot");
		}
		if (player->bombmanager != nullptr)
		{
			App->scene->start_menu->PickItem("bomb");
		}
	}
	// ----------------------------------------------------------------------

	//SET POKEMON UI -------------------------------------------------
	else if (Check.world == "Pokemon")
	{
		App->gui->SetGui(POKEMON_HUD);
		SetItemsinUI();
		poke_hud->RefreshMoney();
	}
	//----------------------------------------------------------------


	Load_new_map(Check.map_id, true);

	return true;
}

void j1Scene::SetMusics(int id)
{
	if (id == 7 && last_map != 8)
	{
		App->audio->PlayMusic("audio/music/POKEMON/PokemonVictoryRoad.ogg", 0);
	}

	if (id == 9)
	{
		App->audio->PlayMusic("audio/music/POKEMON/PokemonEverGrandeCity.ogg", 0);
	}

	if (id == 10)
	{
		App->audio->PlayMusic("audio/music/POKEMON/PokemonPokemonCenter.ogg", 0);
	}

	if (id >= 11 && id < 16)
	{
		App->audio->PlayMusic("audio/music/POKEMON/PokemonVictoryRoad.ogg", 0);
	}

	if (id == 17)
	{
		App->audio->PlayMusic("audio/music/POKEMON/PokemonRivalBattle.ogg", 0);
	}

	if (id >= 18 && id < 22)
	{
		App->audio->PlayMusic("audio/music/POKEMON/PokemonElitteFourBattle.ogg", 0);
	}

	if (id == 22)
	{
		App->audio->PlayMusic("audio/music/POKEMON/PokemonChampionBattle.ogg", 0);
	}

	if (id == 16)
	{
		App->audio->PlayMusic("audio/music/ZELDA/ZeldaLastChamber.ogg", 0);
	}
	App->audio->VolumeMusic(50);
}


bool j1Scene::Load_new_map(int n, bool isTP)
{
	if (player == NULL)
	{
		player = App->entity_elements->CreatePlayer();
	}
	if (Link_die)
	{
		Link_die = false;
		player->SetNormalLife();
	}

	if (n == 9 || n == 10)
	{
		if (n == 10)
		{
			player->Unequip();
			player->sword_equiped = false;
		}
	}

	if (n == 16)
	{
		App->gui->SetGui(ZELDA_HUD);
		player->sword_equiped = true;
	}

	//Stemusic
	SetMusics(n);

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
			LOG("LOAD");
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

			//Change UI when lose combat or win but on this code only when lose
			if (player->lose_combat)
			{
				App->gui->SetGui(POKEMON_HUD);
				player->lose_combat = false;
			}

			//EditCost (Only in EliteFour)
			if (player->state_complet == true)
			{
				App->gui->SetGui(POKEMON_HUD);
				for (pugi::xml_node editcost = temp.child("editcost").child("edit"); editcost != NULL; editcost = editcost.next_sibling())
				{
					App->map->EditCost(editcost.attribute("pos_x").as_int(0), editcost.attribute("pos_y").as_int(0), 0);
				}

				if (n == 9)
				{
					for (pugi::xml_node editcost = temp.child("deledit").child("edit"); editcost != NULL; editcost = editcost.next_sibling())
					{
						App->map->EditCost(editcost.attribute("pos_x").as_int(0), editcost.attribute("pos_y").as_int(0), App->map->data.tilesets[0]->firstgid + 1);
					}
				}
				player->state_complet = false;
			}
			else
			{
				notrepeatCombat = true;
			}

			if (n == 3 && player->bow != nullptr)
			{
				for (pugi::xml_node editcost = temp.child("editcost").child("edit"); editcost != NULL; editcost = editcost.next_sibling())
				{
					App->map->EditCost(editcost.attribute("pos_x").as_int(0), editcost.attribute("pos_y").as_int(0), 0);
				}
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

	//Save CheckPoint Stats ---
	player->SaveCheckPoint(n);
	// --------------------------

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

	if (last_map == 9)
	{
		if (n == 17)
		{
			player->Unequip();
			player->sword_equiped = false;
		}
	}

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
			//all collider nullptr
			App->combatmanager->BeforePrepareCombat();
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
			for (pugi::xml_node editcost = temp.child("deletecost").child("delete_edit"); editcost != NULL; editcost = editcost.next_sibling())
			{
				App->map->EditCost(editcost.attribute("pos_x").as_int(0), editcost.attribute("pos_y").as_int(0), 0);
			}

			//Load UI (?)
			stop_rearch = true;
		}
	}
	//set timer
	App->combatmanager->comprovate.Start();
	App->gui->SetGui(POKEMON_COMBAT);
	return true;
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