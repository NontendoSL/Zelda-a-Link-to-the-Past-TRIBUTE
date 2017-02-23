#include "j1AnimationManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Player.h"

j1AnimationManager::j1AnimationManager()
{
}

j1AnimationManager::~j1AnimationManager()
{
}

bool j1AnimationManager::Awake(pugi::xml_node & test)
{

	return false;
}

bool j1AnimationManager::Start()
{
	link->PushBack({ 0,25,16,24 });
	link->PushBack({ 17,25,16,23 });
	link->PushBack({ 34,25,16,22 });
	link->PushBack({ 51,25,16,22 });
	link->PushBack({ 68,25,16,22 });
	link->PushBack({ 85,25,16,23 });
	link->PushBack({ 102,25,16,24 });
	link_anim.push_back(link);
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
			//App->render->Blit(App->scene->player->player_texture, App->scene->player->position.x, App->scene->player->position.y, (SDL_Rect*)link->GetCurrentFrame());
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