#include "j1DynamicObjects.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"

DynamicObjects::DynamicObjects() :SceneElement()
{
	type = DYNOBJECT;
	name = "dynObjects";
}

DynamicObjects::~DynamicObjects()
{

}

bool DynamicObjects::Awake(pugi::xml_node &conf, uint id, iPoint pos)
{
	bool stop_search = false;
	pugi::xml_node temp = conf.child("dynobjects").child("dynobject");
	if (temp != NULL)
	{
		for (int s_id = temp.attribute("id").as_int(0); stop_search == false; s_id = temp.attribute("id").as_int(0))
		{
			if (id == s_id)
			{
				name = temp.attribute("name").as_string("");
				std::string es = temp.attribute("file").as_string("");
				texture = App->tex->Load(es.c_str());
				position.x = pos.x;
				position.y = pos.y;
				rect = { temp.attribute("rect_x").as_int(0), temp.attribute("rect_y").as_int(0), temp.attribute("rect_w").as_int(0), temp.attribute("rect_h").as_int(0) };
				canBlit = true;
				item_id = temp.attribute("item_id").as_int(0);
				stop_search = true;
			}
			temp = temp.next_sibling();
		}
	}
	return true;
}

bool DynamicObjects::Start()
{
	collision = App->collision->AddCollider({ position.x, position.y, 16, 16 }, COLLIDER_DYNOBJECT, this);
	return true;
}

bool DynamicObjects::Update()
{
	return true;
}

void DynamicObjects::Draw()
{
	BROFILER_CATEGORY("Draw_DynObjects", Profiler::Color::Moccasin)
	if (canBlit == true)
	{
		App->render->Blit(texture, position.x, position.y, &rect);
	}
}

bool DynamicObjects::CleanUp()
{
	return true;
}
