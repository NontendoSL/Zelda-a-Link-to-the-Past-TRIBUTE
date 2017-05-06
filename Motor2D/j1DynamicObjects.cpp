#include "j1DynamicObjects.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityElementsScene.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Audio.h"

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
				pickable = temp.attribute("pickable").as_bool(false);
				stop_search = true;
			}
			temp = temp.next_sibling();
		}
	}
	return true;
}

bool DynamicObjects::Start()
{
	if(name != "door")
	{
		if (name == "bigchest") //Only created if player picked the item inside
		{
			collision = App->collision->AddCollider({ position.x, position.y, 32, 24 }, COLLIDER_DYNOBJECT, this);
		}
		else
		{
			collision = App->collision->AddCollider({ position.x, position.y, 16, 16 }, COLLIDER_DYNOBJECT, this);
		}
	}

	if (pickable == true) //Set variables of throwing and impacting
	{
		speed = 200;
		lifetime = 0.3;
	}

	return true;
}

bool DynamicObjects::Update(float dt)
{
	if (to_follow != nullptr && state == D_PICKED)
	{
		position.x = to_follow->position.x - to_follow->offset_x;
		position.y = to_follow->position.y - to_follow->offset_y - 20;
		collision->SetPos(position.x, position.y);
	}

	else if (state == D_AIR)
	{
		if (timer.ReadSec() >= lifetime && state != D_DYING)
		{
			state = D_DYING;
		}

		else
		{
			KeepGoing(dt);
			IsImpact(App->scene->player->GetFloor()); //Check for wall impact
			collision->SetPos(position.x, position.y);
		}
	}

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

//MODIFY TILE COST OF THE DYNOBJECT POSITION
void DynamicObjects::ModifyTileCost(int cost)
{
	iPoint pos_dyn = App->map->WorldToMap(position.x,position.y);

	App->map->EditCost(pos_dyn.x, pos_dyn.y, cost);
	App->map->EditCost(pos_dyn.x + 1, pos_dyn.y, cost);
	App->map->EditCost(pos_dyn.x, pos_dyn.y + 1, cost);
	App->map->EditCost(pos_dyn.x + 1, pos_dyn.y + 1, cost);
}

bool DynamicObjects::Follow(SceneElement* entity)
{
	if (entity != nullptr)
	{
		to_follow = entity;
	}
	return false;
}

void DynamicObjects::Throw(Direction dir)
{
	state = D_AIR;
	direction = dir;
	timer.Start();
}

void DynamicObjects::KeepGoing(float dt)
{
	switch (direction)
	{
	case UP:
		position.y -= ceil(speed*dt);
		break;
	case DOWN:
		position.y += ceil(speed*dt);
		break;
	case LEFT:
		position.x -= ceil(speed*dt);
		break;
	case RIGHT:
		position.x += ceil(speed*dt);
		break;
	default:
		break;
	}
}

DynObjectState DynamicObjects::IsImpact(int actual_floor)
{
	const MapLayer* meta_layer = App->map->data.layers[App->map->data.layers.size() - 1]; //WEAPONS LAYER
	iPoint map_pos = App->map->WorldToMap(position.x, position.y);
	int arrow_tile = meta_layer->Get(map_pos.x, map_pos.y);

	const TileSet* tileset = App->map->data.tilesets[0]; // META TILESET

	int first_floor = tileset->firstgid + 7; // PINK TILE
	int second_floor = tileset->firstgid + 9; // BLACK TILE
	int third_floor = tileset->firstgid + 6; // ORANGE TILE

	if (actual_floor == 0)
	{
		if (arrow_tile == first_floor)
		{
			state = D_DYING;
		}
	}
	else if (actual_floor == 1)
	{
		if (arrow_tile == second_floor)
		{
			state = D_DYING;
		}
	}
	else if (actual_floor == 2)
	{
		if (arrow_tile == third_floor)
		{
			state = D_DYING;
		}
	}
	else
	{
		state = D_AIR;
	}

	return state;
}

DynObjectState DynamicObjects::GetState() const
{
	return state;
}

void DynamicObjects::SetState(DynObjectState s_state)
{
	state = s_state;
}

void DynamicObjects::SetAnimState(DynObjectState a_state)
{
	anim_state = a_state;
}
