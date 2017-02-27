#include "j1Enemy.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "p2Defs.h"
#include "j1Input.h"
#include "j1Item.h"

Enemy::Enemy(iPoint position):j1SceneElement(position)
{
	name = "enemies";
	type = ENEMY;
}

Enemy::~Enemy()
{}

bool Enemy::Awake(pugi::xml_node &conf, uint id)
{
	bool stop_search = false;
	for (int s_id = conf.attribute("id").as_int(0); stop_search == false; s_id = conf.next_sibling().attribute("id").as_int(0))
	{
		if (id == s_id)
		{
			std::string temp = conf.attribute("file").as_string("");
			texture = App->tex->Load(temp.c_str());
			hp = conf.attribute("hp").as_int(0);
			/*position.x = conf.child("enemy").attribute("pos_x").as_int(0);
			position.y = conf.child("enemy").attribute("pos_y").as_int(0);*/
			stop_search = true;
		}
	}

	return true;
}

bool Enemy::Start()
{


	return true;
}

bool Enemy::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		position.x -= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		position.y += 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		position.x += 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	{
		position.y -= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		hp -= 2;
	}


	return true;
}

void Enemy::Draw()
{
	if (hp > 0)
	{
		SDL_Rect temp{ 1056,189,66,90 };
		App->render->Blit(texture, position.x, position.y, &temp);
	}
	else
	{
		//Drop_item();
	}
}

void Enemy::AddItem(Item* item)
{
	item_inside = item;
	item->canBlit = false;
}

void Enemy::Drop_item()
{
	item_inside->canBlit = true;
	item_inside->position.x = position.x;
	item_inside->position.y = position.y;
	item_inside = NULL;
}

bool Enemy::CleanUp()
{
	return true;
}

bool Enemy::Save()
{
	return true;
}





