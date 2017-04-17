#include "j1DynamicObjects.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityElementsScene.h"

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
				if (App->entity_elements->texture_dynobjects != nullptr)
				{
					/*std::string es = temp.attribute("file").as_string("");
					texture = App->tex->Load(es.c_str());*/
				}

				position.x = pos.x;
				position.y = pos.y;
				rect = { temp.attribute("rect_x").as_int(0), temp.attribute("rect_y").as_int(0), temp.attribute("rect_w").as_int(0), temp.attribute("rect_h").as_int(0) };
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
	if(name != "door_house_link" && name != "doorinside_Link_south" && name != "roof_Link" && name != "doorVictory_south")
	{
		if (name == "bigchest")
		{
			collision = App->collision->AddCollider({ position.x, position.y, 32, 24 }, COLLIDER_DYNOBJECT, this);
		}
		else
		{
			collision = App->collision->AddCollider({ position.x, position.y, 16, 16 }, COLLIDER_DYNOBJECT, this);
		}
	}
	return true;
}

bool DynamicObjects::Update(float dt)
{
	return true;
}

void DynamicObjects::Draw()
{
	BROFILER_CATEGORY("Draw_DynObjects", Profiler::Color::Moccasin);

	App->render->Blit(App->entity_elements->texture_dynobjects, position.x, position.y, &rect);

}

bool DynamicObjects::CleanUp()
{
	return true;
}
