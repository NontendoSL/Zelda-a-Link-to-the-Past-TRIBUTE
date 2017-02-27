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
#include "j1Player.h"
#include "j1FileSystem.h"

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
	//TODO HIGH -> All proces of to create player has wrong...
	player = App->entity_elements->CreatePlayer(iPoint(100, 100));

	woaw = App->gui->CreateDialogue({ 50,500 }, "Hi Link! Whatsapp Bro?");
	woaw->AddLine("-Ameisin");

	Load_new_map(1);
	switch_map = 0;
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

	/*if (enemy != NULL)//TODO HIGH -> when enemy die on put this code?
	{
		if (enemy->hp == 0)
		{
			enemy->Drop_item();
			App->entity_elements->DeleteEnemy(enemy);
			enemy = NULL;
		}
	}*/

	if (switch_map == 2)
	{
		if (App->map->CleanUp())
		{
			Load_new_map(2);
		}

		switch_map = 0;
	}
	if (switch_map == 1)
	{
		if (App->map->CleanUp())
		{
			Load_new_map(1);
		}

		switch_map = 0;
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

bool j1Scene::Load_new_map(int n)
{
	bool stop_rearch = false;

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);
	for (pugi::xml_node temp = config.child("maps").child("map"); stop_rearch == false; temp = temp.next_sibling())
	{
		if (temp.attribute("n").as_int(0) == n)
		{
			//player position
			player->position.x = temp.child("player").attribute("pos_x").as_int(0);
			player->position.y = temp.child("player").attribute("pos_y").as_int(0);

			//Enemies
			pugi::xml_node temp_enemy = temp.child("enemies").child("enemy");
			for (int i = 0; i < temp.child("enemies").attribute("num").as_int(0); i++)
			{
				enemy.push_back(App->entity_elements->CreateEnemy(iPoint(temp_enemy.attribute("pos_x").as_int(0), temp_enemy.attribute("pos_y").as_int(0)), temp_enemy.attribute("id").as_int(0), temp_enemy));
				temp_enemy = temp_enemy.next_sibling();
			}

			//items
			//Dynamic items (not implmeneted yet)

			//map
			std::string name_map = temp.attribute("file").as_string("");
			App->map->Load(name_map.c_str());

			//Camera position
			App->render->camera.x = 256 / 2 - temp.child("player").attribute("pos_x").as_int(0);
			App->render->camera.y = 224 / 2 - temp.child("player").attribute("pos_y").as_int(0);


			stop_rearch = true;
		}
	}
	

	//enemy = App->entity_elements->CreateEnemy(iPoint(200, 400), 1);
	//items = App->entity_elements->CreateItem(iPoint(300, 200), 1);
	//
	//enemy->AddItem(items);


	return true;
}


// ---------------------------------------------
pugi::xml_node j1Scene::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}