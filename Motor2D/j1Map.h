#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include <list>
#include "p2Point.h"
#include "j1Module.h"
#include "j1AnimationManager.h"

enum TileDirection { T_CONTINUE, T_WALL, T_UP, T_DOWN, T_LEFT, T_RIGHT };

struct DirectionMap
{
	std::string name;
	int id_tile;
	int id_map;
	iPoint position;
};
// ----------------------------------------------------
struct Properties
{
	struct Property
	{
		std::string name;
		int value;
	};

	~Properties()
	{
		//p2List_item<Property*>* item;
		std::list<Property*>::iterator item;
		item = properties.begin();

		while(item != properties.end())
		{
			RELEASE(item._Ptr->_Myval);
			item++;
		}

		properties.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	std::list<Property*> properties;
};

// ----------------------------------------------------
struct MapLayer
{
	std::string name;
	int			width;
	int			height;
	uint*		data;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;

	std::vector<TileSet*> tilesets;
	std::vector<MapLayer*> layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path, uint id_map);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;
	
	TileDirection MovementCost(int x, int y, int oX, int oY, Direction dir) const;
	int CheckTileCost(int x, int y);

	void EditCost(int x, int y,int value);
	void DynObjectFromTiled(uint id);

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	int Checkpositions();

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;

	std::vector<Collider*> doors;
	std::vector<DirectionMap> directMap;
private:

	bool navigation_map = false;
	bool navigation_map_2 = false;
	pugi::xml_document	map_file;
	std::string folder;
	bool				map_loaded;

	iPoint pos_camera;
	iPoint win_size;
};

#endif // __j1MAP_H__