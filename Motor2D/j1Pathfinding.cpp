#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name = "pathfinding";
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

std::vector<iPoint>* j1PathFinding::GetLastPath()
{
	return &last_path;
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path //TODO LOW -> const_iterator??


// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
std::list<PathNode>::iterator PathList::Find(const iPoint& point) 
{
	std::list<PathNode>::iterator item = list.begin();
	while(item != list.cend())
	{
		if(item._Ptr->_Myval.pos == point)
			return item;
		item++;
	}
	item=list.end();
	return item;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
std::list<PathNode>::const_iterator PathList::GetNodeLowestScore() const
{
	//p2List_item<PathNode>* ret = NULL;
	//TODO p1 -> const_iterator?? something wrong...
	std::list<PathNode>::const_iterator ret;
	int min = 65535;

	//p2List_item<PathNode>* item = list.end;
	std::list<PathNode>::const_iterator item = list.end();
	item--;
	while(item != list.begin())
	{
		if(item._Ptr->_Myval.Score() < min)
		{
			min = item._Ptr->_Myval.Score();
			ret = item;
		}
		item++;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL){}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent){}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent){}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.size();

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	return list_to_fill.list.size();
}

uint PathNode::FindWalkableAdjacents_D(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.size();

	// north
	cell.create(pos.x, pos.y + 1);
	iPoint north = cell;
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	iPoint south = cell;
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x - 1, pos.y);
	iPoint east = cell;
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x + 1, pos.y);
	iPoint west = cell;
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// north - west
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
	{
		if (App->pathfinding->IsWalkable(north) == true && App->pathfinding->IsWalkable(west) == true)
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
	}

	// north-east
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
	{
		if (App->pathfinding->IsWalkable(north) == true && App->pathfinding->IsWalkable(east) == true)
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
	}

	// south - west
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
	{
		if (App->pathfinding->IsWalkable(south) == true && App->pathfinding->IsWalkable(west) == true)
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
	}

	// south - east
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
	{
		if (App->pathfinding->IsWalkable(south) == true && App->pathfinding->IsWalkable(east) == true)
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
	}


	return list_to_fill.list.size();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

float PathNode::CalculateF_D(const iPoint & destination)
{
	bool is_Diagonal = false;
	if (pos.x != parent->pos.x && pos.y != parent->pos.y)
	{
		is_Diagonal = true;
	}

	if (is_Diagonal == true)
	{
		g = parent->g + sqrtf(2);
	}

	else
	{
		g = parent->g + 1;
	}

	h = pos.DistanceManhattan(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	// TODO 1: if origin or destination are not walkable, return -1
	if (IsWalkable(origin) == false || IsWalkable(destination) == false)
	{
		return -1;
	}

	// TODO 2: Create three lists: open, close
	// Add the origin tile to open
	PathList open;
	PathList close;

	open.list.push_back(PathNode(0, origin.DistanceManhattan(destination), origin, nullptr));

	// Iterate while we have tile in the open list
	while (open.list.size() > 0)
	{
		// TODO 3: Move the lowest score cell from open list to the closed list
		close.list.push_back(*open.GetNodeLowestScore());
		open.list.erase(open.GetNodeLowestScore());

		// TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		if (close.list.back().pos == destination)
		{
			last_path.clear();
			std::list<PathNode>::const_iterator item = close.list.end();
			item--;
			for (; item->parent != nullptr; item = close.Find(item->parent->pos))
			{
				last_path.push_back(item->pos);
			}
			last_path.push_back(origin);
			std::reverse(last_path.begin(), last_path.end()); //FLIP
			return last_path.size();
		}

		// TODO 5: Fill a list of all adjancent nodes
		else
		{
			PathList neighbors;
			close.list.back().FindWalkableAdjacents(neighbors);

			// TODO 6: Iterate adjancent nodes:
			// ignore nodes in the closed list
			// If it is NOT found, calculate its F and add it to the open list
			// If it is already in the open list, check if it is a better path (compare G)
			// If it is a better path, Update the parent
			std::list<PathNode>::iterator item;
			iPoint dest = destination;
			for (item = neighbors.list.begin(); item != neighbors.list.end(); item++)
			{
				if (close.Find(item->pos) == close.list.end())
				{
					item->CalculateF(dest);
					if (open.Find(item->pos) == close.list.end())
					{
						open.list.push_back(*item);
					}
					else
					{
						if (item->g < open.Find(item->pos)->g)
						{
							open.Find(item->pos)->parent = item->parent;
						}
					}
				}
			}
			neighbors.list.clear();
		}
	}
}



