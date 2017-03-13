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
#include "Soldier.h"
#include "j1Player.h"
#include "j1DynamicObjects.h"
#include "j1FileSystem.h"
#include "j1Collision.h"

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
	//UI
	charge = App->gui->CreateImage({ 20,18,16,42 }, { 20,18 });
	item = App->gui->CreateImage({ 37,20,22,22 }, { 37,20 });
	gems = App->gui->CreateImage({ 72,15,8,8 }, { 72,15 });
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { -7,10 }));
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 1,10 }));
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 9,10 }));
	bombs = App->gui->CreateImage({ 100,15,8,8 }, { 100,15 });
	arrows = App->gui->CreateImage({ 121,15,14,8 }, { 121,15 });
	life = App->gui->CreateImage({ 178,15,44,7}, { 178,15 });
	/*dialog = App->gui->CreateDialogue({ 40,150 }, "Hi Link! Whatsapp Bro?");
	dialog->AddLine("-Ameisin");
	dialog->AddLine("U wot m8");
	dialog->AddLine("visible test");
	dialog->AddLine("nontendo switch");*/

	//Create First level
	player = App->entity_elements->CreatePlayer();
	App->map->Load("TiledLinkHouse.tmx");
	int scale = App->win->GetScale();
	App->render->camera.x = -((player->position.x - (256 / 2)) * scale);//TODO LOW -> No Magic Numbers
	App->render->camera.y = -((player->position.y - (224 / 2)) * scale);

	//Load_new_map(1);
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
	AssignValues(gems);
	App->map->Draw();
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			App->render->camera.x += 2;
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			App->render->camera.y -= 2;
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			App->render->camera.y += 2;
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			App->render->camera.x -= 2;
	}

	if (enemy.size() > 0 && enemy.begin()._Ptr->_Myval != NULL)//TODO HIGH -> when enemy die on put this code?
	{
		if (enemy.begin()._Ptr->_Myval->hp == 0)
		{
			items.push_back(App->entity_elements->CreateItem(1));
			enemy.begin()._Ptr->_Myval->AddItem(items.begin()._Ptr->_Myval);
			enemy.begin()._Ptr->_Myval->Drop_item();
			App->entity_elements->DeleteEnemy(enemy.begin()._Ptr->_Myval);
			enemy.begin()._Ptr->_Myval = NULL;
		}
	}

	//enemy.push_back(App->entity_elements->CreateEnemy(iPoint(80, 70), 1));
	if (switch_map == 2)
	{
		//TODO need destroy all enemies and items
		if (App->map->CleanUp())
		{
			Load_new_map(2);
			/*dynitems.push_back(App->entity_elements->CreateDynObject(iPoint(176, 245), 3));
			dynitems.push_back(App->entity_elements->CreateDynObject(iPoint(224, 273), 4));
			dynitems.push_back(App->entity_elements->CreateDynObject(iPoint(224, 289), 4));
			dynitems.push_back(App->entity_elements->CreateDynObject(iPoint(240, 273), 4));
			dynitems.push_back(App->entity_elements->CreateDynObject(iPoint(240, 289), 4));*/
		}

		switch_map = 0;
	}
	if (switch_map == 1)
	{
		if (App->map->CleanUp())
		{
			App->collision->EreseAllColiderPlayer();
			App->entity_elements->DelteElements();
			//App->entity_elements->DeleteEnemy(enemy.begin()._Ptr->_Myval);
			
			std::list<Soldier*>::iterator item = enemy.begin();
			if (enemy.size() > 0)
			{
				while (item != enemy.end())
				{
					enemy.pop_back();
					item++;
				}
				enemy.clear();
			}
			std::list<Item*>::iterator item_s = items.begin();
			if (items.size() > 0)
			{
				while (item_s != items.end())
				{
					items.pop_back();
					item_s++;
				}
				items.clear();
			}
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

void j1Scene::AssignValues(Image* assigner)
{
	std::list<Image*>::iterator value = assigner->elements.end();
	value--;
	int number = player->gems % 10;
	value._Ptr->_Myval->AssignNumber(number);
	value--;
	number = player->gems / 10;
	number %= 10;
	value._Ptr->_Myval->AssignNumber(number);
	value--;
	number = player->gems / 100;
	value._Ptr->_Myval->AssignNumber(number);
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
				enemy.push_back(App->entity_elements->CreateSoldier(temp_enemy.attribute("id").as_int(0), temp_enemy));
				temp_enemy = temp_enemy.next_sibling();
			}

			//items
			//Dynamic items (not implmeneted yet)

			//map
			std::string name_map = temp.attribute("file").as_string("");
			App->map->Load(name_map.c_str());

			//Camera position
			int scale = App->win->GetScale();
			App->render->camera.x = -((temp.child("player").attribute("pos_x").as_int(0) - (256 / 2)) * scale);
			App->render->camera.y = -((temp.child("player").attribute("pos_y").as_int(0) - (224 / 2)) * scale);


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