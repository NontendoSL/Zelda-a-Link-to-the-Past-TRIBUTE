#include "j1EntityElementsScene.h"
#include "Soldier.h"
#include "j1Item.h"
#include "j1Player.h"
#include "j1DynamicObjects.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FileSystem.h"
#include "j1Weapon.h"


j1EntityElementScene::j1EntityElementScene()
{
	name = "entityelement";
}

j1EntityElementScene::~j1EntityElementScene()
{
}

bool j1EntityElementScene::Awake(pugi::xml_node &config)
{
	std::list<SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{
		//TODO HIGH -> we have to do that in Awake call enemies, items and dynitems and load all textures and save this textures in std::vector<Textures*>
		//item._Ptr->_Myval->Awake(config.child(item._Ptr->_Myval->name.c_str()));
		item++;
	}

	return true;
}

bool j1EntityElementScene::Start()
{
	bool ret = true;
	std::list<SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{
		//item._Ptr->_Myval->Start();
		item++;
	}
	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	XML.load_buffer(buf, size);
	return ret;
}

bool j1EntityElementScene::PreUpdate()
{
	BROFILER_CATEGORY("DoUpdate_Elements", Profiler::Color::Cyan);

	return true;
}

bool j1EntityElementScene::Update(float dt)
{
	bool ret = true;
	BROFILER_CATEGORY("DoUpdate_Elements", Profiler::Color::Cyan);
	if (App->scene->combat == false)
	{
		std::list<SceneElement*>::iterator item3 = elementscene.begin();
		while (item3 != elementscene.end())
		{
			item3._Ptr->_Myval->Update();
			item3++;
		}
	}

	return ret;
}

bool j1EntityElementScene::PostUpdate()
{
	BROFILER_CATEGORY("Draw_Elements", Profiler::Color::Green);
	if (App->scene->combat == false)
	{
		std::list<SceneElement*>::iterator item = elementscene.end();
		item--;
		while (item != elementscene.begin()) //TODO HIGH -> need inverse_iterator
		{
			item._Ptr->_Myval->Draw();
			item--;
		}
		if (elementscene.size() > 0)
		{
			item._Ptr->_Myval->Draw();
		}
	}
	return true;
}

bool j1EntityElementScene::CleanUp()
{
	bool ret = true;
	std::list<SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{
		item._Ptr->_Myval->CleanUp();
		item++;
	}
	return ret;
}

bool j1EntityElementScene::DelteElements()
{
	std::list<SceneElement*>::iterator item = elementscene.end();
	item--;
	if (elementscene.size() > 1)
	{
		while (item != elementscene.begin())
		{
			delete item._Ptr->_Myval;
			elementscene.pop_back();
			item--;
		}
	}
	return true;
}

Soldier* j1EntityElementScene::CreateSoldier(uint id, pugi::xml_node& config)
{

	Soldier* element = new Soldier();
	element->Awake(config, id);
	element->Start();
	elementscene.push_back(element);

	return element;
}

bool j1EntityElementScene::DeleteEnemy(Soldier* enemy)
{
	if (enemy != nullptr)
	{
		elementscene.remove(enemy);
		App->scene->enemy.remove(enemy);
		enemy->collision_enemy->to_delete = true;
		enemy = nullptr;
		delete enemy;
		LOG("Enemy DELETE!");
	}

	return true;
}

bool j1EntityElementScene::DeleteDynObject(DynamicObjects* dynobject)
{
	elementscene.remove(dynobject);
	App->scene->dynobjects.remove(dynobject);
	dynobject->collision->to_delete = true;
	dynobject = nullptr;
	delete dynobject;

	LOG("DynObject DELETE!");
	return true;
}

bool j1EntityElementScene::DeleteItem(Item* item)
{
	elementscene.remove(item);
	App->scene->items.remove(item);
	item->collision->to_delete = true;
	item = nullptr;
	delete item;
	LOG("Item DELETE!");
	return true;
}

Item* j1EntityElementScene::CreateItem(uint id, iPoint position)
{

	Item* element = new Item();
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);
	element->Awake(config.child(element->name.c_str()), id, position);
	element->Start();
	elementscene.push_back(element);

	return element;
}

Hookshot* j1EntityElementScene::CreateHookshot()
{
	Hookshot* hook = new Hookshot(true);
	hook->name = "hookshot";
	hook->Start();
	elementscene.push_back(hook);
	return hook;
}

DynamicObjects* j1EntityElementScene::CreateDynObject(iPoint pos, uint id, uint id_map)
{
	DynamicObjects* element = new DynamicObjects();
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);
	bool stop_rearch = false;
	LOG("Create DynObjects");
	config = config.child("maps").child("map");
	for (; stop_rearch == false; config = config.next_sibling())
	{
		if (config.attribute("n").as_int(0) == id_map)
		{
			element->Awake(config, id, pos);
			element->Start();
			elementscene.push_back(element);
			LOG("Created!!");
			stop_rearch = true;
		}
	}


	return element;
}

Player* j1EntityElementScene::CreatePlayer()
{
	Player* element = new Player();

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);
	element->Awake(config.child(element->name.c_str()));
	element->Start();

	elementscene.push_back(element);

	return element;
}

// ---------------------------------------------
pugi::xml_node j1EntityElementScene::LoadConfig(pugi::xml_document& config_file) const
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