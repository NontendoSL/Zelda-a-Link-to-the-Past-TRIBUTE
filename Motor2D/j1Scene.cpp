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
	}
	inventory = false;
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
	if (ingame==true)
	{
		AssignValues(gems, player->gems);
		AssignValues(bombs, player->bombs);
		AssignValues(arrows, player->arrows);
		force->Hitbox.w = player->charge;
		App->map->Draw();


		if (enemy.size() > 0 && enemy.begin()._Ptr->_Myval != NULL)//TODO HIGH -> when enemy die on put this code?
		{
			if (enemy.begin()._Ptr->_Myval->hp == 0)
			{
				//items.push_back(App->entity_elements->CreateItem(1));
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
				App->collision->EreseAllColiderPlayer();
				App->entity_elements->DelteElements();
				if (dynobjects.size() > 0)
				{
					dynobjects.clear();
				}

				Load_new_map(2);
				/*dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(176, 245), 3));
				dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(224, 273), 4));
				dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(224, 289), 4));
				dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(240, 273), 4));
				dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(240, 289), 4));*/
			}

			switch_map = 0;
		}
		if (switch_map == 1)
		{
			if (App->map->CleanUp())
			{
				App->collision->EreseAllColiderPlayer();
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
				Load_new_map(1);
			}
			switch_map = 0;
		}

		if (switch_menu)
		{
			SwitchMenu(!inventory);
		}
	}
	

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}
	if (inventory==true&&ingame==true)
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			start_menu->Select(1);
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			start_menu->Select(-1);
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			start_menu->Click();
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			start_menu->UnClick(); //TODO LOW-> fix mini bugs
		}
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
	hud = new Menu();
	Image* Sprite = App->gui->CreateImage({ 18,44,42,16 }, { 12,35 }, "charge");
	force = App->gui->CreateImage({ 21,61,34,10 }, { 4,3 }, "force");
	Sprite->elements.push_back(force);
	hud->AddElement(Sprite);//[0] adding charge bar
	hud->AddElement(App->gui->CreateImage({ 37,20,22,22 }, { 22,12 }, "item"));
	gems = App->gui->CreateImage({ 72,15,8,8 }, { 72,15 }, "gems"); //Gems and value
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { -7,10 }));
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 1,10 }));
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 9,10 }));
	hud->AddElement(gems); //adding gems [2]
	bombs= App->gui->CreateImage({ 100,15,8,8 }, { 100,15 }, "bombs");
	bombs->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { -3,9 }));
	bombs->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 5,9 }));
	hud->AddElement(bombs);// adding bombs
	arrows= App->gui->CreateImage({ 121,15,14,8 }, { 121,15 },"arrows");
	arrows->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { -3,9 }));
	arrows->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 5,9 }));
	hud->AddElement(arrows);//adding arrows
	hud->AddElement(App->gui->CreateImage({ 178,15,44,7 }, { 178,15 }, "life"));
	hud->position = { 0,0 };
	//Start Menu
	start_menu = new Menu();
	Sprite = App->gui->CreateImage({ 1,255,256,224 }, { 0,-224 }, "bg");
	Button* hotfix = App->gui->CreateButton({ 271,268,32,32 }, { 24,21-224 }, { 304,268 }, { 337,268 }, false, "1stITEM");
	hotfix->selected = true;
	start_menu->AddElement(hotfix);
	start_menu->AddElement(App->gui->CreateButton({ 271,301,32,32 }, { 48,21-224 }, { 304,301 }, { 337,301 },false, "2ndITEM"));
	start_menu->AddElement(Sprite);
	start_menu->position = { 0,-224 };
	start_menu->Close();
//	hotfix->identifier = "JODER";
}

void j1Scene::SwitchMenu(bool direction)//true for down, false for up
{
	if (direction)
	{
		if (start_menu->position.y < 0)
		{
			start_menu->Open();
			start_menu->Move(false, 2.0);
			hud->Move(false, 2.0);
		}
		else
		{
			hud->Close();
			switch_menu = false;
			inventory = true;
			player->gamestate = INMENU;
		}
	}
	else
	{
		if (hud->position.y > 0)
		{
			hud->Open();
			start_menu->Move(false, -2.0);
			hud->Move(false, -2.0);
		}
		else
		{
			start_menu->Close();
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

	bool stop_rearch = false;

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);

	for (pugi::xml_node temp = config.child("maps").child("map"); stop_rearch == false; temp = temp.next_sibling())
	{
		if (temp.attribute("n").as_int(0) == n)
		{
			//player position
			player->position.x = temp.child("Link").attribute("pos_x").as_int(0);
			player->position.y = temp.child("Link").attribute("pos_y").as_int(0);

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

			//Camera position
			int scale = App->win->GetScale();
			if (temp.child("camera").attribute("follow").as_bool() == true)
			{
				App->render->camera.x = -((temp.child("Link").attribute("pos_x").as_int(0) - (256 / 2)) * scale);
				App->render->camera.y = -((temp.child("Link").attribute("pos_y").as_int(0) - (224 / 2)) * scale);
			}
			else
			{
				iPoint size_pos = App->map->MapToWorld(App->map->data.width, App->map->data.height);
				App->render->camera.x = (256 - size_pos.x);
				App->render->camera.y = (224 - size_pos.y);
			}
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
	int size = App->fs->Load("Levels.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("levels");

	return ret;
}