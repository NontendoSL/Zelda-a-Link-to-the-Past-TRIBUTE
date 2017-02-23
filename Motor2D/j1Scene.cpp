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
#include "j1Enemy.h"

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
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	charge = App->gui->CreateImage({ 20,18,16,42 }, { 20,18 });
	item = App->gui->CreateImage({ 37,20,22,22 }, { 37,20 });
	gems = App->gui->CreateImage({ 72,15,8,8 }, { 72,15 });
	test = App->gui->CreateText("A link to the Nintendo Worlds", { 12,80 }, 23);
	//TODO HIGH -> All proces of to create player has wrong...
	player = App->entity_elements->CreatePlayer(iPoint(100, 100));
	enemy = App->entity_elements->CreateEnemy(iPoint(200, 400), 1);
	items = App->entity_elements->CreateItem(iPoint(300, 200), 1);
	enemy->AddItem(items);
	woaw = App->gui->CreateDialogue({ 50,500 }, "Hi Link! Whatsapp Bro?");
	woaw->AddLine("-Ameisin");
	App->map->Load("iso.tmx");
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
	App->map->Draw();
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		//TODO LOW -> Directions + and - Inverse?? 
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			App->render->camera.x += 2;
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			App->render->camera.y -= 2;
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			App->render->camera.y += 2;
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			App->render->camera.x -= 2;
	}

	if (enemy != NULL)//TODO HIGH -> when enemy die on put this code?
	{
		if (enemy->hp == 0)
		{
			enemy->Drop_item();
			App->entity_elements->DeleteEnemy(enemy);
			enemy = NULL;
		}
	}



	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
