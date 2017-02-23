#include "j1AnimationManager.h"

j1AnimationManager::j1AnimationManager()
{
}

j1AnimationManager::~j1AnimationManager()
{
}

bool j1AnimationManager::Awake(pugi::xml_node &)
{
	return false;
}

bool j1AnimationManager::Start()
{
	return false;
}

bool j1AnimationManager::Update(float dt)
{
	return false;
}

bool j1AnimationManager::CleanUp()
{
	return false;
}

void j1AnimationManager::PlayerSelector(AnimationState status, Direction dir) 
{
	if (dir == TOP)
	{
		if (status == IDLE)
		{

		}
		if (status == ATTACK)
		{

		}
		if (status == WALKING)
		{

		}
	}
	else if (dir == BOT)
	{
		if (status == IDLE)
		{

		}
		if (status == ATTACK)
		{

		}
		if (status == WALKING)
		{

		}
	}
	else if (dir == LEFT)
	{
		if (status == IDLE)
		{

		}
		if (status == ATTACK)
		{

		}
		if (status == WALKING)
		{

		}
	}
	else if (dir == RIGHT)
	{
		if (status == IDLE)
		{

		}
		if (status == ATTACK)
		{

		}
		if (status == WALKING)
		{

		}
	}

}