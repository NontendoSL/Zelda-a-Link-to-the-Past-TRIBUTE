#include "j1Item.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Collision.h"


Item::Item() :SceneElement()
{
	name = "items";
	type = ITEM;
}

Item::~Item()
{}

bool Item::Awake(pugi::xml_node &conf, uint item_id, iPoint pos)
{
	bool stop_search = false;
	for (pugi::xml_node temp = conf.child("item"); stop_search == false; temp = temp.next_sibling())
	{
		if (temp.attribute("id").as_int(0) == item_id)
		{
			id = item_id;
			name = temp.attribute("name").as_string("");
			position.x = pos.x;
			position.y = pos.y;
			std::string es = temp.attribute("file").as_string("");
			texture = App->tex->Load(es.c_str());
			stop_search = true;
		}
	}
	return true;
}


bool Item::Start()
{
	int width = 0;
	int	height = 0;

	//ITEM MANAGEMENT (1 -> RUPEE // 2 -> BOMB // 3-> HOOKSHOT // 4-> HEART)
	if (id == 1)
	{
		width = 8;
		height = 14;
	}
	else if (id == 2)
	{
		width = 12;
		height = 15;
	}
	else if (id == 3)
	{
		width = 7;
		height = 16;
	}
	else if (id == 4)
	{
		width = 14;
		height = 13;
	}
	delay.Start();
	collision = App->collision->AddCollider({ position.x, position.y, width, height }, COLLIDER_ITEM, this);
	return true;
}

bool Item::Update(float dt)
{
	if (delay.ReadSec() >= 0.5)
	{
		pickable = true;
	}
	return true;
}

void Item::Draw()
{
	App->render->Blit(texture, position.x, position.y);
}

bool Item::CleanUp()
{
	return true;
}

bool Item::Save()
{
	return true;
}
