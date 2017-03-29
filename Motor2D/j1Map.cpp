#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Scene.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name="map";
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;
	navigation_map = false;
	folder=config.child("folder").child_value();

	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("Draw_MAP", Profiler::Color::DarkOrange)
	if(map_loaded == false)
		return;

	//p2List_item<MapLayer*>* item = data.layers.start;
	std::list<MapLayer*>::iterator item = data.layers.begin();

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		navigation_map = !navigation_map;

	for(; item != data.layers.end(); item++)
	{
		MapLayer* layer = item._Ptr->_Myval;

		if(layer->properties.Get("Draw") != 0 && navigation_map == false)
			continue;

		/*iPoint temp = WorldToMap(-(App->render->camera.x /2), -(App->render->camera.y/2));
		iPoint te_size = WorldToMap(512/2, 448/2);*/
		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if(tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = MapToWorld(x, y);

					App->render->Blit(tileset->texture, pos.x, pos.y, &r);
				}
			}
		}
	}
}

int Properties::Get(const char* value, int default_value) const
{
	std::list<Property*>::const_iterator item = list.begin();
	while(item != list.end())
	{
		if(strcmp(item._Ptr->_Myval->name.c_str(), value) == 0)
			return item._Ptr->_Myval->value;
		item++;
	}

	return default_value;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	//p2List_item<TileSet*>* item = data.tilesets.start;
	std::list<TileSet*>::const_iterator item = data.tilesets.begin();
	TileSet* set = item._Ptr->_Myval;

	while(item != data.tilesets.end())
	{
		if(id < item._Ptr->_Myval->firstgid)
		{
			item--;
			set = item._Ptr->_Myval;
			break;
		}
		set = item._Ptr->_Myval;
		item++;
	}

	return set;
}

void j1Map::EditCost(int x, int y, int value)
{
	std::list<MapLayer*>::const_iterator item = data.layers.end();
	item--;
	item._Ptr->_Myval->data[(y*item._Ptr->_Myval->width) + x] = value;
}

int j1Map::MovementCost(int x, int y, Direction dir) const
{
	int ret = 0;
	int red_wal = data.tilesets.begin()._Ptr->_Next->_Myval->firstgid + 1;
	int blue_wal = red_wal + 7;
	if (dir == UP)
	{
		iPoint ptemp = WorldToMap(x, y);
		iPoint ptemp_2 = WorldToMap(x + 8, y);
		iPoint ptemp_3 = WorldToMap(x + 15, y);
		std::list<MapLayer*>::const_iterator item = data.layers.end();
		item--;
		int id_1 = item._Ptr->_Myval->Get(ptemp.x, ptemp.y);
		int id_2 = item._Ptr->_Myval->Get(ptemp_2.x, ptemp_2.y);
		int id_3 = item._Ptr->_Myval->Get(ptemp_3.x, ptemp_3.y);
		
		if (id_1 == red_wal && id_2 == 0 && id_3 == 0)
		{
			ret = 4;
		}
		else if (id_1 == 0 && id_2 == 0 && id_3 == red_wal)
		{
			ret = 3;
		}
		else if (id_1 == red_wal || id_2 == red_wal || id_3 == red_wal)
		{
			ret = 1;
		}
		else if (id_1 == blue_wal)//TODO HIGH-> Only player enter
		{
			App->scene->switch_map = 1;
		}
		else
			ret = 0;

	}
	if (dir == LEFT)
	{
		iPoint ptemp = WorldToMap(x, y);
		iPoint ptemp_2 = WorldToMap(x, y + 8);
		iPoint ptemp_3 = WorldToMap(x, y + 15);
		std::list<MapLayer*>::const_iterator item = data.layers.end();
		item--;
		int id_1 = item._Ptr->_Myval->Get(ptemp.x, ptemp.y);
		int id_2 = item._Ptr->_Myval->Get(ptemp_2.x, ptemp_2.y);
		int id_3 = item._Ptr->_Myval->Get(ptemp_3.x, ptemp_3.y);

		if (id_1 == red_wal && id_2 == 0 && id_3 == 0)
		{
			ret = 4;//4 -> down
		}
		else if (id_1 == 0 && id_2 == 0 && id_3 == red_wal)
		{
			ret = 3;//3-> up
		}
		else if (id_1 == red_wal || id_2 == red_wal || id_3 == red_wal)
		{
			ret = 1;
		}
		else
			ret = 0;
	}
	if (dir == RIGHT)
	{
		iPoint ptemp = WorldToMap(x, y);
		iPoint ptemp_2 = WorldToMap(x, y + 8);
		iPoint ptemp_3 = WorldToMap(x, y + 15);
		std::list<MapLayer*>::const_iterator item = data.layers.end();
		item--;
		int id_1 = item._Ptr->_Myval->Get(ptemp.x, ptemp.y);
		int id_2 = item._Ptr->_Myval->Get(ptemp_2.x, ptemp_2.y);
		int id_3 = item._Ptr->_Myval->Get(ptemp_3.x, ptemp_3.y);

		if (id_1 == red_wal && id_2 == 0 && id_3 == 0)
		{
			ret = 3;
		}
		else if (id_1 == 0 && id_2 == 0 && id_3 == red_wal)
		{
			ret = 4;
		}
		else if (id_1 == red_wal || id_2 == red_wal || id_3 == red_wal)
		{
			ret = 1;
		}
		else
			ret = 0;
	}
	if (dir == DOWN)
	{
		iPoint ptemp = WorldToMap(x, y);
		iPoint ptemp_2 = WorldToMap(x + 8, y);
		iPoint ptemp_3 = WorldToMap(x + 15, y);
		std::list<MapLayer*>::const_iterator item = data.layers.end();
		item--;
		int id_1 = item._Ptr->_Myval->Get(ptemp.x, ptemp.y);
		int id_2 = item._Ptr->_Myval->Get(ptemp_2.x, ptemp_2.y);
		int id_3 = item._Ptr->_Myval->Get(ptemp_3.x, ptemp_3.y);

		if (id_1 == red_wal && id_2 == 0 && id_3 == 0)
		{
			ret = 4;
		}
		else if (id_1 == 0 && id_2 == 0 && id_3 == red_wal)
		{
			ret = 3;
		}

		else if (id_1 == red_wal || id_2 == red_wal || id_3 == red_wal)
		{
			ret = 1;
		}
		else if (id_1 == blue_wal)//TODO HIGH-> Only player enter
		{
			App->scene->switch_map = 2;
		}
		else
			ret = 0;
	}

	return ret;
}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		
		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int( (x / half_width + y / half_height) / 2) - 1;
		ret.y = int( (y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	//p2List_item<TileSet*>* item;
	std::list<TileSet*>::iterator item;
	item = data.tilesets.begin();
	while(item != data.tilesets.end())
	{
		App->tex->UnLoad(item._Ptr->_Myval->texture);
		RELEASE(item._Ptr->_Myval);
		item++;
	}
	data.tilesets.clear();

	// Remove all layers
	//p2List_item<MapLayer*>* item2;
	std::list<MapLayer*>::iterator item2;
	item2 = data.layers.begin();

	while(item2 != data.layers.end())
	{
		RELEASE(item2._Ptr->_Myval);
		item2++;
	}
	data.layers.clear();



	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name, uint id_map)
{
	BROFILER_CATEGORY("Load_Map", Profiler::Color::OrangeRed)
	bool ret = true;
	std::string tmp(folder.c_str());
	tmp += file_name;

	char* buf;
	int size = App->fs->Load(tmp.c_str(), &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.layers.push_back(lay);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		//p2List_item<TileSet*>* item = data.tilesets.start;
		std::list<TileSet*>::iterator item = data.tilesets.begin();
		while(item != data.tilesets.end())
		{
			TileSet* s = item._Ptr->_Myval;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.c_str(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item++;
		}

		//p2List_item<MapLayer*>* item_layer = data.layers.start;
		std::list<MapLayer*>::iterator item_layer = data.layers.begin();
		while(item_layer != data.layers.end())
		{
			MapLayer* l = item_layer._Ptr->_Myval;
			LOG("Layer ----");
			LOG("name: %s", l->name.c_str());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer++;
		}
	}
	//TODO Create all DynObjects from Tiled
	if (id_map > 0)
	{
		DynObjectFromTiled(id_map);
	}


	map_loaded = ret;

	return ret;
}

void j1Map::DynObjectFromTiled(uint id_map)
{
	int yellowid_1 = data.tilesets.begin()._Ptr->_Next->_Myval->firstgid + 2;
	int yellowid_2 = data.tilesets.begin()._Ptr->_Next->_Myval->firstgid + 3;
	int yellowid_3 = data.tilesets.begin()._Ptr->_Next->_Myval->firstgid + 4;
	int yellowid_4 = data.tilesets.begin()._Ptr->_Next->_Myval->firstgid + 5;

	std::list<MapLayer*>::const_iterator item = data.layers.end();
	item--;

	for (int y = 0; y < data.height; ++y)
	{
		for (int x = 0; x < data.width; ++x)
		{
			int tile_id = item._Ptr->_Myval->Get(x, y);
			iPoint positionObject = MapToWorld(x, y);
			if (tile_id == yellowid_1)
			{
				LOG("DynObject 1");
				App->scene->dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(positionObject.x, positionObject.y), 1, id_map));
				EditCost(x, y, data.tilesets.begin()._Ptr->_Next->_Myval->firstgid + 1);
			}
			if (tile_id == yellowid_2)
			{
				LOG("DynObject 2");
				App->scene->dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(positionObject.x, positionObject.y), 2, id_map));
				EditCost(x, y, data.tilesets.begin()._Ptr->_Next->_Myval->firstgid + 1);
			}
			if (tile_id == yellowid_3)
			{
				LOG("DynObject 3");
				App->scene->dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(positionObject.x, positionObject.y), 3, id_map));
				EditCost(x, y, data.tilesets.begin()._Ptr->_Next->_Myval->firstgid + 1);
			}
			if (tile_id == yellowid_4)
			{
				LOG("DynObject 4");
				App->scene->dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(positionObject.x, positionObject.y), 4, id_map));
				EditCost(x, y, data.tilesets.begin()._Ptr->_Next->_Myval->firstgid + 1);
			}
		}
	}
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		std::string bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.length() > 0)
		{
			std::string red, green, blue;
			red=bg_color.substr(1, 2);
			green=bg_color.substr(3, 4);
			blue=bg_color.substr(5, 6);

			int v = 0;

			sscanf_s(red.c_str(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.c_str(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.c_str(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}
		std::string orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name=tileset_node.attribute("name").as_string(0);
	set->firstgid = tileset_node.attribute("firstgid").as_int(0);
	set->tile_width = tileset_node.attribute("tilewidth").as_int(0);
	set->tile_height = tileset_node.attribute("tileheight").as_int(0);
	set->margin = tileset_node.attribute("margin").as_int(0);
	set->spacing = tileset_node.attribute("spacing").as_int(0);
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.c_str(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / (set->tile_width + set->margin);
		set->num_tiles_height = set->tex_height / (set->tile_height + set->margin);
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if(data != NULL)
	{
		pugi::xml_node prop;

		for(prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.push_back(p);
		}
	}

	return ret;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	//p2List_item<MapLayer*>* item;
	std::list<MapLayer*>::const_iterator item;
	item = data.layers.begin();

	for(; item != data.layers.end(); item++)
	{
		MapLayer* layer = item._Ptr->_Myval;

		if(layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;
				
				if(tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
						map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}