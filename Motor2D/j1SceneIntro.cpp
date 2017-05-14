#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1InputManager.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1GuiElements.h"
#include "j1Scene.h"
#include "j1SceneIntro.h"
#include "Soldier.h"
#include "j1Player.h"
#include "j1DynamicObjects.h"
#include "j1FileSystem.h"
#include "j1Collision.h"
#include "j1AnimationManager.h"
#include "Video.h"

j1SceneIntro::j1SceneIntro() : j1Module()
{
	name = "scene_intro";
}

// Destructor
j1SceneIntro::~j1SceneIntro()
{}

// Called before render is available
bool j1SceneIntro::Awake()
{
	LOG("Loading SceneIntro");
	bool ret = true;
	return ret;
}

// Called before the first frame
bool j1SceneIntro::Start()
{
	TitleScreen_letters = App->tex->Load("gui/title_screen/letters.png");
	TitleScreen_bg = App->tex->Load("gui/title_screen/bg_anim.png"); //TODO LOW -> .png
	Menu_bg = App->tex->Load("gui/title_screen/menu_bg.png");
	Menu_Cursor = App->audio->LoadFx("audio/fx/LTTP_Menu_Cursor.wav");
	App->input_manager->AddListener(this);
	SDL_Rect r = { 0, 0, 640, 480 };
	App->video->PlayVideo("Untitled.ogv", r);
	fade = true;
	return true;
}

// Called each loop iteration
bool j1SceneIntro::PreUpdate()
{


	return true;
}

// Called each loop iteration
bool j1SceneIntro::Update(float dt)
{
	bool ret = true;
	if (App->video->video_finished)
	{
		if (clenaudio_video == false)
		{
			clenaudio_video = true;
			App->audio->CleanUp();
			App->audio->Awake(pugi::xml_node(nullptr));
			App->audio->Start();
			App->audio->PlayMusic("audio/music/ZELDA/ZeldaScreenSelection.ogg");
		}
		if (App->scene->ingame == false)
		{
			if (menu == false)
			{
				if (bg_anim < -180) {
					right = true;
				}
				if (bg_anim > 0) {
					right = false;
				}
				if (right)
				{
					bg_anim += 0.3;
				}
				else
				{
					bg_anim -= 0.3;
				}
				App->render->Blit(TitleScreen_bg, 0, 0, NULL, NULL, false, NULL, NULL, NULL, { bg_anim,0 });
				App->render->Blit(TitleScreen_letters, 50, 10, NULL, NULL, false);
			}
			else
			{
				if (bg_anim > -70) {
					bg_anim -= 0.2;
				}
				App->render->Blit(Menu_bg, 0, 0, NULL, NULL, false, NULL, NULL, NULL, { bg_anim,0 });
				App->render->Blit(TitleScreen_letters, -10, 0, NULL, NULL, false);

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
		}
		if (goHouse)
		{
			if (fade)
			{
				App->fadetoblack->FadeToBlack(3);
				App->audio->FadeMusic(3);
				fade = false;
			}
			else
			{
				if (App->fadetoblack->Checkfadetoblack())
				{
					App->scene->ingame = true;
					App->scene->Start();
					goHouse = false;
				}
			}
		}


		if (App->scene->ingame == false)
		{

			if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			{
				ret = false;
			}
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_START) == EVENTSTATE::E_DOWN)
			{
				if (menu == false)
				{
					LoadMainMenu();
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP || App->input_manager->EventPressed(INPUTEVENT::BUTTON_START) == EVENTSTATE::E_UP)
			{
				if (menu == false)
				{
					menu = true;
					bg_anim = 0;
					TitleScreen_letters = App->tex->Load("gui/title_screen/letters_menu.png");
					App->gui->SetGui(MAIN_MENU);
				}
			}
		}
	}
	
	return ret;
	
}

// Called each loop iteration
bool j1SceneIntro::PostUpdate()
{
	return true;
}

void j1SceneIntro::OnGui(j1GuiEntity* element, GuiAction event)
{
	
	if (element->identifier == "Newgame_b")
	{
		if (event == GuiAction::CLICK_DOWN)
		{
			if (menu == true)
			{
				((Button*)element)->click = true;
				App->audio->PlayFx(Menu_Cursor);
			}
		}
		else if (event == GuiAction::CLICK_UP)
		{
			if (menu == true)
			{
				main_menu->active = false;
				goHouse = true;
				((Button*)element)->click = false;
				App->scene->new_game = true;
				return;
			}
		}
	}
	if (element->identifier == "Continue_b")
	{
		if (event == GuiAction::CLICK_DOWN)
		{
			if (menu == true)
			{
				((Button*)element)->click = true;
				App->audio->PlayFx(Menu_Cursor);
			}
		}
		else if (event == GuiAction::CLICK_UP)
		{
			if (menu == true)
			{
				main_menu->active = false;
				goHouse = true;
				((Button*)element)->click = false;

				if (App->fs->Exists("save/save_file.xml"))
				{
					App->LoadGame("save_file.xml");
				}
				App->scene->continue_game = true;
				return;
			}
		}
	}
	if (element->identifier == "Quitgame_b")
	{
		if (event == GuiAction::CLICK_DOWN)
		{
			if (menu == true)
			{
				((Button*)element)->click = true;
				App->audio->PlayFx(Menu_Cursor);
			}
		}
		else if (event == GuiAction::CLICK_UP)
		{
			if (menu == true)
			{
				((Button*)element)->click = false;
				App->scene->update_return = false;
			}
		}
	}
	if (element->identifier == "Options_b")
	{
		if (event == GuiAction::CLICK_DOWN)
		{
			if (menu == true)
			{
				((Button*)element)->click = true;
				App->audio->PlayFx(Menu_Cursor);
			}
		}
		else if (event == GuiAction::CLICK_UP)
		{
			if (menu == true)
			{
				((Button*)element)->click = false;
				App->gui->SetGui(MAPPING_CONTROLLER);
				main_menu->active = false;
			}
		}
	}
	if (element->identifier == "normal_controller_but")
	{
		if (event == GuiAction::CLICK_DOWN)
		{
			((Button*)element)->click = true;
			((Button*)App->gui->GetEntity("tactic_controller_but"))->click = false;
			App->input_manager->ChangeInput(true);
			App->scene->use_tatical = false;
		}
	}
	if (element->identifier == "tactic_controller_but")
	{
		if (event == GuiAction::CLICK_DOWN)
		{
			((Button*)element)->click = true;
			((Button*)App->gui->GetEntity("normal_controller_but"))->click = false;
			App->input_manager->ChangeInput(false);
			App->scene->use_tatical = true;
		}
	}
}

void j1SceneIntro::LoadMainMenu()
{
	main_menu = App->gui->CreateMainMenu();
	controller_map = App->gui->CreateControllerMapping();
}


// Called before quitting
bool j1SceneIntro::CleanUp()
{
	LOG("Freeing sceneintro");

	return true;
}