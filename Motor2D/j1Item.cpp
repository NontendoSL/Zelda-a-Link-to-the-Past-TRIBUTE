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

bool Item::Awake(pugi::xml_node &conf, uint id, iPoint pos)
{
	bool stop_search = false;
	for (int s_id = conf.child("item").attribute("id").as_int(0); stop_search == false; s_id = conf.child("item").next_sibling().attribute("id").as_int(0))
	{
		if (id == s_id)
		{
			name = conf.child("item").attribute("name").as_string("");
			position.x = pos.x;
			position.y = pos.y;
			std::string es = conf.child("item").attribute("file").as_string("");
			texture = App->tex->Load(es.c_str());
			canBlit = true;
			stop_search = true;
		}
	}

	return true;
}

bool Item::Start()
{
	collision = App->collision->AddCollider({ position.x, position.y, 8,20 }, COLLIDER_ITEM, this);
	return true;
}

bool Item::Update()
{


	return true;
}

void Item::Draw()
{
	if (canBlit == true)
	{
		App->render->Blit(texture, position.x, position.y);
	}
}

bool Item::CleanUp()
{
	return true;
}

bool Item::Save()
{
	return true;
}


