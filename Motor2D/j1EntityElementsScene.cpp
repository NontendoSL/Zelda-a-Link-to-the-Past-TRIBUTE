#include "j1EntityElementsScene.h"
#include "j1Enemy.h"
#include "j1Item.h"
#include "j1Player.h"

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
		item._Ptr->_Myval->Awake(config.child(item._Ptr->_Myval->name.c_str()));
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
		item._Ptr->_Myval->Start();
		item++;
	}
	return ret;
}

bool j1EntityElementScene::Update(float dt)
{
	bool ret = true;
	std::list<j1SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{
		item._Ptr->_Myval->Update();
		item++;
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

j1Enemy* j1EntityElementScene::CreateEnemy(iPoint position, int hp, int attack, float speed) 
{

	j1Enemy* element = new j1Enemy(position, hp, attack, speed);
	elementscene.push_back(element);

	return element;
}

j1Item* j1EntityElementScene::CreateItem(iPoint position) 
{

	j1Item* element = new j1Item(position);
	elementscene.push_back(element);

	return element;
}

Player* j1EntityElementScene::CreatePlayer(iPoint position)
{
	Player* element = new Player(position);
	elementscene.push_back(element);

	return element;
}