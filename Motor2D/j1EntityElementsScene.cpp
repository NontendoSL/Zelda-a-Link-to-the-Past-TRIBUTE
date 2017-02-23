#include "j1EntityElementsScene.h"
#include "j1Enemy.h"
#include "j1Item.h"
#include "j1Player.h"
#include "j1App.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FileSystem.h"


j1EntityElementScene::j1EntityElementScene()
{
	name = "entityelement";
}

j1EntityElementScene::~j1EntityElementScene()
{
}

bool j1EntityElementScene::Awake(pugi::xml_node &config)
{
	std::list<j1SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{
		//item._Ptr->_Myval->Awake(config.child(item._Ptr->_Myval->name.c_str()));
		item++;
	}

	return true;
}

bool j1EntityElementScene::Start()
{
	bool ret = true;
	std::list<j1SceneElement*>::iterator item = elementscene.begin();
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

bool j1EntityElementScene::Update(float dt)
{
	bool ret = true;

	std::list<j1SceneElement*>::iterator item3 = elementscene.begin();
	while (item3 != elementscene.end())
	{
		item3._Ptr->_Myval->Update();
		item3++;
	}
	return ret;
}

bool j1EntityElementScene::CleanUp()
{
	bool ret = true;
	std::list<j1SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{
		item._Ptr->_Myval->CleanUp();
		item++;
	}
	return ret;
}

Enemy* j1EntityElementScene::CreateEnemy(iPoint position, uint id)
{

	Enemy* element = new Enemy(position);
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);
	element->Awake(config.child(element->name.c_str()), id);
	element->Start();
	elementscene.push_back(element);

	return element;
}

bool j1EntityElementScene::DeleteEnemy(Enemy * enemy)
{
	elementscene.remove(enemy);
	LOG("Enemy DELETE!");
	return true;
}

Item* j1EntityElementScene::CreateItem(iPoint position, uint id) 
{

	Item* element = new Item(position);
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);
	element->Awake(config.child(element->name.c_str()), id);
	element->Start();
	elementscene.push_back(element);

	return element;
}

Player* j1EntityElementScene::CreatePlayer(iPoint position)
{
	Player* element = new Player(position);

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
	int size = App->fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}