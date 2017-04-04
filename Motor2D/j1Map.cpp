#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Window.h"
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

void j1Map::Draw()  // TODO LOW -> maybe change pointers to const(?)
{
	BROFILER_CATEGORY("Draw_MAP", Profiler::Color::DarkOrange)
	if(map_loaded == false)
		return;


	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		navigation_map = !navigation_map;
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		navigation_map_2 = !navigation_map_2;

	for (uint i = 0; i < data.layers.size(); i++)
	{
		MapLayer* layer = data.layers[i];

		if(layer->properties.Get("NoDraw") != 0 && navigation_map == false)
			continue;
		if (layer->properties.Get("NoDraw_2") != 0 && navigation_map_2 == false)
			continue;

		int marge = Checkpositions();
		for(int y = pos_camera.y; y < pos_camera.y + win_size.y + marge; ++y)
		{
			for(int x = pos_camera.x; x < pos_camera.x + win_size.x + marge; ++x)
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

int j1Map::Checkpositions()
{
	int ret = 0;
	int scale = App->win->GetScale();
	pos_camera = WorldToMap(-(App->render->camera.x / scale), -(App->render->camera.y / scale));
	win_size = WorldToMap(App->win->GetWidth() / scale, App->win->GetHeight() / scale);

	if (pos_camera.x < 0)
	{
		pos_camera.x = 0;
	}
	if (pos_camera.y < 0)
	{
		pos_camera.y = 0;
	}
	if (win_size.x > data.width)
	{
		win_size.x = data.width;
	}
	else
	{
		ret = 1;
	}
	if (win_size.y > data.height)
	{
		win_size.y = data.height;
	}
	else
	{
		ret = 1;
	}

	return ret;
}



int Properties::Get(const char* value, int default_value) const
{
	std::list<Property*>::const_iterator item = properties.begin();
	while(item != properties.end())
	{
		if(strcmp(item._Ptr->_Myval->name.c_str(), value) == 0)
			return item._Ptr->_Myval->value;
		item++;
	}

	return default_value;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	TileSet* set = data.tilesets[0];
	for (uint i = 0; i < data.tilesets.size(); i++)
	{
		if (id < data.tilesets[i]->firstgid)
		{
			set = data.tilesets[i - 1];
			break;
		}
		set = data.tilesets[i];
	}
	return set;
}

void j1Map::EditCost(int x, int y, int value)
{
	MapLayer* meta_layer = data.layers[1];
	meta_layer->data[y*meta_layer->width + x] = value;
}

TileDirection j1Map::MovementCost(int x, int y, int width, int height, Direction dir) const
{
	TileDirection ret = T_CONTINUE;
	int red_wal = data.tilesets[1]->firstgid + 1; // RED TILE

	if (dir == UP)
	{
		iPoint ptemp = WorldToMap(x, y); //left position
		iPoint ptemp_2 = WorldToMap(x + width*0.5, y); //central position
		iPoint ptemp_3 = WorldToMap(x + width, y); //rigth position

		MapLayer* meta_layer = data.layers[1];

		int id_1 = meta_layer->Get(ptemp.x, ptemp.y);
		int id_2 = meta_layer->Get(ptemp_2.x, ptemp_2.y);
		int id_3 = meta_layer->Get(ptemp_3.x, ptemp_3.y);
		
		if (id_1 == red_wal && id_2 == 0 && id_3 == 0)
		{
			ret = T_RIGHT;
		}
		else if (id_1 == 0 && id_2 == 0 && id_3 == red_wal)
		{
			ret = T_LEFT;
		}
		else if (id_1 == red_wal || id_2 == red_wal || id_3 == red_wal)
		{
			ret = T_WALL; 
		}
		else
			ret = T_CONTINUE;

	}
	if (dir == LEFT)
	{
		iPoint ptemp = WorldToMap(x, y); //up position
		iPoint ptemp_2 = WorldToMap(x, y + height*0.5); //central position
		iPoint ptemp_3 = WorldToMap(x, y + height); //down position

		MapLayer* meta_layer = data.layers[1];

		int id_1 = meta_layer->Get(ptemp.x, ptemp.y);
		int id_2 = meta_layer->Get(ptemp_2.x, ptemp_2.y);
		int id_3 = meta_layer->Get(ptemp_3.x, ptemp_3.y);

		if (id_1 == red_wal && id_2 == 0 && id_3 == 0)
		{
			ret = T_DOWN;//4 -> down
		}
		else if (id_1 == 0 && id_2 == 0 && id_3 == red_wal)
		{
			ret = T_UP;//3-> up
		}
		else if (id_1 == red_wal || id_2 == red_wal || id_3 == red_wal)
		{
			ret = T_WALL;
		}
		else
			ret = T_CONTINUE;
	}
	if (dir == RIGHT)
	{
		iPoint ptemp = WorldToMap(x, y); //up position
		iPoint ptemp_2 = WorldToMap(x, y + width*0.5); //central position
		iPoint ptemp_3 = WorldToMap(x, y + width); //down position

		MapLayer* meta_layer = data.layers[1];

		int id_1 = meta_layer->Get(ptemp.x, ptemp.y);
		int id_2 = meta_layer->Get(ptemp_2.x, ptemp_2.y);
		int id_3 = meta_layer->Get(ptemp_3.x, ptemp_3.y);

		if (id_1 == red_wal && id_2 == 0 && id_3 == 0)
		{
			ret = T_DOWN; //DOWN
		}
		else if (id_1 == 0 && id_2 == 0 && id_3 == red_wal)
		{
			ret = T_UP; //UP
		}
		else if (id_1 == red_wal || id_2 == red_wal || id_3 == red_wal)
		{
			ret = T_WALL;
		}
		else
			ret = T_CONTINUE;
	}
	if (dir == DOWN)
	{
		iPoint ptemp = WorldToMap(x, y); //left position
		iPoint ptemp_2 = WorldToMap(x + width*0.5, y); //central position
		iPoint ptemp_3 = WorldToMap(x + width, y); //down position

		MapLayer* meta_layer = data.layers[1];

		int id_1 = meta_layer->Get(ptemp.x, ptemp.y);
		int id_2 = meta_layer->Get(ptemp_2.x, ptemp_2.y);
		int id_3 = meta_layer->Get(ptemp_3.x, ptemp_3.y);

		if (id_1 == red_wal && id_2 == 0 && id_3 == 0)
		{
			ret = T_RIGHT;
		}
		else if (id_1 == 0 && id_2 == 0 && id_3 == red_wal)
		{
			ret = T_LEFT;
		}

		else if (id_1 == red_wal || id_2 == red_wal || id_3 == red_wal)
		{
			ret = T_WALL;
		}
		else
			ret = T_CONTINUE;
	}


	return ret;
}

int j1Map::CheckTileCost(int x, int y)
{
	int ret = 0;
	int red_wal = data.tilesets[1]->firstgid + 1; // RED TILE

	iPoint ptemp = WorldToMap(x, y);

	MapLayer* meta_layer = data.layers[1]; //Walkability Layer
	int id = meta_layer->Get(ptemp.x, ptemp.y);

	if (id == red_wal)
	{
		int ret = 1;
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
	for (uint i = 0; i < data.tilesets.size(); i++)
	{
		App->tex->UnLoad(data.tilesets[i]->texture);
		RELEASE(data.tilesets[i]);
	}
	data.tilesets.clear();

	// Remove all layers
	for (uint i = 0; i < data.layers.size(); i++)
	{
		RELEASE(data.layers[i]);
	}
	data.layers.clear();

	// Remove all DirectionMap
	directMap.clear();
	doors.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;

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

		for (uint i = 0; i < data.tilesets.size(); i++)
		{
			TileSet* s = data.tilesets[i];
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.c_str(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
		}

		for (uint i = 0; i < data.layers.size(); i++)
		{
			MapLayer* l = data.layers[i];
			LOG("Layer ----");
			LOG("name: %s", l->name.c_str());
			LOG("tile width: %d tile height: %d", l->width, l->height);
		}
	}

	//Create DirectionMap
	for (pugi::xml_node temp = map_file.child("map").child("directionLevels").child("map"); temp != NULL; temp = temp.next_sibling())
	{
		DirectionMap dir_map;
		dir_map.name = temp.attribute("name").as_string("");
		dir_map.id_tile = temp.attribute("id_tile").as_int(0);
		dir_map.id_map = temp.attribute("id_map").as_int(0);
		dir_map.position = iPoint(temp.attribute("pos_x").as_int(0), temp.attribute("pos_y").as_int(0));
		directMap.push_back(dir_map);
	}

	//Create all DynObjects from Tiled
	if (id_map > 0)
	{
		DynObjectFromTiled(id_map);
	}


	map_loaded = ret;

	return ret;
}

void j1Map::DynObjectFromTiled(uint id_map)
{
	int blue = data.tilesets[1]->firstgid + 8;
	int green = data.tilesets[1]->firstgid;
	int orange = data.tilesets[1]->firstgid + 6;
	int purple = data.tilesets[1]->firstgid + 7;

	MapLayer* temp = data.layers[1];

	for (int y = 0; y < data.height; ++y)
	{
		for (int x = 0; x < data.width; ++x)
		{
			int tile_id = temp->Get(x, y);
			iPoint positionObject = MapToWorld(x, y);
			if (tile_id >= data.tilesets[1]->firstgid + 2 && tile_id <= data.tilesets[1]->firstgid + 5 ||
				tile_id >= data.tilesets[1]->firstgid + 10 && tile_id <= data.tilesets[1]->firstgid + 15)
			{
				if ((id_map == 4 && App->scene->player->hook != nullptr && tile_id == data.tilesets[1]->firstgid + 10) ||
					(id_map == 5 && App->scene->player->bombmanager != nullptr && tile_id == data.tilesets[1]->firstgid + 10))
				{
					//DON'T CREATE AGAIN THE BIG CHEST TODO HIGH -> REMODELATE THIS METHOD!!!!
				}
				else
				{
					if (tile_id >= data.tilesets[1]->firstgid + 10)
					{
						App->scene->dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(positionObject.x, positionObject.y), tile_id - data.tilesets[1]->firstgid - 5, id_map));
						if (tile_id - data.tilesets[1]->firstgid - 5 == 7)
						{
							EditCost(x, y, 0);
						}
						else
							EditCost(x, y, data.tilesets[1]->firstgid + 1);
					}
					else
					{
						App->scene->dynobjects.push_back(App->entity_elements->CreateDynObject(iPoint(positionObject.x, positionObject.y), tile_id - data.tilesets[1]->firstgid - 1, id_map));
						EditCost(x, y, data.tilesets[1]->firstgid + 1);
					}
				}

			}
			if (tile_id == blue)
			{
				blue = -1;
				doors.push_back(App->collision->AddCollider(SDL_Rect{ positionObject.x, positionObject.y, 15, 15 }, COLLIDER_SWITCH_MAP));
			}
			if (tile_id == green)
			{
				green = -1;
				doors.push_back(App->collision->AddCollider(SDL_Rect{ positionObject.x, positionObject.y, 15, 15 }, COLLIDER_SWITCH_MAP));
			}
			if (tile_id == purple)
			{
				purple = -1;
				doors.push_back(App->collision->AddCollider(SDL_Rect{ positionObject.x, positionObject.y, 15, 15 }, COLLIDER_SWITCH_MAP));
			}
			if (tile_id == orange)
			{
				orange = -1;
				doors.push_back(App->collision->AddCollider(SDL_Rect{ positionObject.x, positionObject.y, 15, 15 }, COLLIDER_SWITCH_MAP));
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

			properties.properties.push_back(p);
		}
	}

	return ret;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	//p2List_item<MapLayer*>* item;


	for (uint i = 0; i < data.layers.size(); i++)
	{
		MapLayer* layer = data.layers[i];
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