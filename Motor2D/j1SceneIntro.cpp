#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1InputManager.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
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

j1SceneIntro::j1SceneIntro() : j1Module()
{
	name = "scene";
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
	TitleScreen_bg = App->tex->Load("gui/title_screen/bg_anim.jpg");
	Menu_bg= App->tex->Load("gui/title_screen/menu_bg.jpg");
	App->audio->PlayMusic("audio/music/ZELDA/ZeldaScreenSelection.ogg");
	App->input_manager->AddListener(this);
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
	if (App->scene->ingame == false)
	{
		if (menu == false)
		{
			if (bg_anim < -120) {
				right = true;
			}
			if (bg_anim > 0) {
				right = false;
			}
			if (right)
			{
				bg_anim += 0.4;
			}
			else
			{
				bg_anim -= 0.4;
			}
			App->render->Blit(TitleScreen_bg, 0, 0, NULL, NULL, false, NULL, NULL, NULL, { bg_anim,0 });
			App->render->Blit(TitleScreen_letters, 0, 0, NULL, NULL, false);
		}
		else
		{
			if (bg_anim > -70) {
				bg_anim -= 0.2;
			}
			App->render->Blit(Menu_bg, 0, 0, NULL, NULL, false, NULL, NULL, NULL, { bg_anim,0 });
			App->render->Blit(TitleScreen_letters, 0, 0, NULL, NULL, false);
					
		}
	}

	return true;
}

// Called each loop iteration
bool j1SceneIntro::PostUpdate()
{
	bool ret = true;
	if (App->scene->ingame == false )
	{
		if (menu)
		{
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT)
			{
				main_menu->Select(1);
			}
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
			{
				main_menu->Select(-1);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			ret = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			if (menu == false)
			{
				menu = true;
				bg_anim = 0;
				TitleScreen_letters = App->tex->Load("gui/title_screen/letters_menu.png");
				LoadMainMenu();
			}
			if (main_menu->id_selected == 1)
			{
				App->audio->FadeMusic(2);
				App->scene->ingame = true;
				App->scene->Start();
				main_menu->Close();
			}
		}
	}
	
	return ret;
}

void j1SceneIntro::LoadMainMenu()
{
	main_menu = new Menu();
	Button* menu_button=App->gui->CreateButton({ 1,146,110,17 }, { 172 / 2,180 / 2 }, { 0,0 }, { 112,164 }, true);
	menu_button->selected = true;
	menu_button->anim->PushBack({ 112,146,110,17 });
	menu_button->anim->PushBack({ 223,146,110,17 });
	menu_button->anim->PushBack({ 334,146,110,17 });
	menu_button->anim->PushBack({ 1,164,110,17 });
	menu_button->anim->PushBack({ 334,146,110,17 });
	menu_button->anim->PushBack({ 223,146,110,17 });
	menu_button->anim->speed = 0.25f;
	menu_button->resize = false;
	main_menu->AddElement(menu_button);
	menu_button= App->gui->CreateButton({ 1,182,125,17 }, { 172 / 2,210 / 2 }, { 0,0 }, { 127,200 }, true);
	menu_button->anim->PushBack({ 127,182,125,17 });
	menu_button->anim->PushBack({ 253,182,125,17 });
	menu_button->anim->PushBack({ 379,182,125,17 });
	menu_button->anim->PushBack({ 1,200,125,17 });
	menu_button->anim->PushBack({ 379,182,125,17 });
	menu_button->anim->PushBack({ 253,182,125,17 });
	menu_button->anim->speed = 0.25f;
	menu_button->resize = false;
	main_menu->AddElement(menu_button);
	menu_button = App->gui->CreateButton({ 1,218,110,17 }, { 172 / 2, 240/ 2 }, { 0,0 }, { 112,236 }, true);
	menu_button->anim->PushBack({ 112,218,110,17 });
	menu_button->anim->PushBack({ 223,218,110,17 });
	menu_button->anim->PushBack({ 334,218,110,17 });
	menu_button->anim->PushBack({ 1,236,110,17 });
	menu_button->anim->PushBack({ 334,218,110,17 });
	menu_button->anim->PushBack({ 223,218,110,17 });
	menu_button->anim->speed = 0.25f;
	menu_button->resize = false;
	main_menu->AddElement(menu_button);
}

void j1SceneIntro::OnInputCallback(INPUTEVENT action, EVENTSTATE state)
{
	if (App->scene->ingame == false)
	{
		switch (action)
		{
		case MUP:
			if (menu == true)
			{
				if (state == E_DOWN)
					main_menu->Select(-1);
			}
			break;

		case MDOWN:
			if (menu == true)
			{
				if (state == E_DOWN)
					main_menu->Select(1);
			}
			break;

		case BUTTON_B:
			if (state == E_DOWN)
			{
				if (menu == false)
				{
					menu = true;
					bg_anim = 0;
					TitleScreen_letters = App->tex->Load("gui/title_screen/letters_menu.png");
					LoadMainMenu();
				}
				if (main_menu->id_selected == 1)
				{
					App->audio->FadeMusic(2);
					App->scene->ingame = true;
					App->scene->Start();
					main_menu->Close();
				}
			}
			break;
		}
	}
}

// Called before quitting
bool j1SceneIntro::CleanUp()
{
	LOG("Freeing sceneintro");

	return true;
}