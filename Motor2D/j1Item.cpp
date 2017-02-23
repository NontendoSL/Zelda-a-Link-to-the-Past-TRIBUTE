#include "j1Item.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1App.h"


Item::Item(iPoint position) :j1SceneElement(position)
{
	name = "items";
	type = ITEM;
}

Item::~Item()
{}

bool Item::Awake(pugi::xml_node &conf, uint id)
{
	bool stop_search = false;
	for (int s_id = conf.child("item").attribute("id").as_int(0); stop_search == false; s_id = conf.child("item").next_sibling().attribute("id").as_int(0))
	{
		if (id == s_id)
		{
			name = conf.child("item").attribute("name").as_string("");
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
		SDL_Rect tem = { 1128,189,66,90 };
		App->render->Blit(texture, position.x, position.y, &tem);
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


