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
#include "j1GuiEntity.h"
#include "j1GuiElements.h"
#include "j1Scene.h"
#include "j1SceneIntro.h"
#include "Soldier.h"
#include "j1Player.h"
#include "j1DynamicObjects.h"
#include "j1FileSystem.h"
#include "j1Collision.h"

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
	App->audio->PlayMusic("audio/music/ZELDA/ZeldaScreenSelection.ogg");
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
		if (bg_anim < -120) {
			right = true;
		}
		if (bg_anim > 0) {
			right = false;
		}
		if (right)
		{
			bg_anim += 0.5;
		}
		else
		{
			bg_anim -= 0.5;
		}
		App->render->Blit(TitleScreen_bg, bg_anim, 0, NULL, NULL, false);
		App->render->Blit(TitleScreen_letters, 0, 0, NULL, NULL, false);
	}

	return true;
}

// Called each loop iteration
bool j1SceneIntro::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->audio->FadeMusic(2);
		App->scene->ingame = true;
		App->scene->Start();
	}
	return ret;
}

// Called before quitting
bool j1SceneIntro::CleanUp()
{
	LOG("Freeing sceneintro");

	return true;
}