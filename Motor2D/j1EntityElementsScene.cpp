#include "j1EntityElementsScene.h"
#include "j1Enemy.h"
#include "j1Item.h"

j1EntityElementScene::j1EntityElementScene()
{
}

j1EntityElementScene::~j1EntityElementScene()
{
}

bool j1EntityElementScene::Awake(pugi::xml_node &)
{
	return false;
}

bool j1EntityElementScene::Start()
{
	return false;
}

bool j1EntityElementScene::Update(float dt)
{
	return false;
}

bool j1EntityElementScene::CleanUp()
{
	return false;
}

j1Enemy* j1EntityElementScene::CreateEnemy(iPoint position, int hp, int attack, float speed) {

	j1Enemy* element = new j1Enemy(position, hp, attack, speed);
	elementscene.push_back(element);

	return element;
}

j1Item* j1EntityElementScene::CreateItem(iPoint position) {

	j1Item* element = new j1Item(position);
	elementscene.push_back(element);

	return element;
}