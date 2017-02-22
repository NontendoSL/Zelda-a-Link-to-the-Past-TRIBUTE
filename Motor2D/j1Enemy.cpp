#include "j1Enemy.h"


j1Enemy::j1Enemy(iPoint position, int hp, int attack, float speed):j1SceneElement(position)
{
	type = ENEMY;
}

j1Enemy::~j1Enemy()
{}

bool j1Enemy::Update()
{

	return true;
}

void j1Enemy::Draw()
{


}

