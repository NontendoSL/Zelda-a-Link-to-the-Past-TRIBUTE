#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include <vector>
#include <queue>

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255

struct PathNode;

class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	~j1PathFinding();

	// Called before quitting
	bool CleanUp();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);

	// Main function to request a path from A to B
	int CreatePath(const iPoint& origin, const iPoint& destination);

	// To request all tiles involved in the last generated path
	std::vector<iPoint>* GetLastPath();

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;

	// Return a pointer of the node at pos (x,y)
	PathNode* GetPathNode(int x, int y);

private:

	// size of the map
	uint width;
	uint height;

	PathNode* map_node = nullptr;

	// all map walkability values [0..255]
	uchar* map;

	// we store the created path here
	std::vector<iPoint> last_path;
};

// forward declaration
struct PathList;

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------
struct PathNode
{
	// Convenient constructors
	//PathNode();
	PathNode(int g, int h, const iPoint& pos, PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathList& list_to_fill) const;

	// Fills a list (PathList) of all valid adjacent pathnodes with all cardinal points
	uint FindWalkableAdjacents_D(PathList& list_to_fill) const;

	// Calculates this tile score
	int Score() const;

	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);
	float CalculateF_D(const iPoint& destination);

	// -----------
	int g;
	int h;
	iPoint pos;
	PathNode* parent = nullptr; // needed to reconstruct the path in the end

	bool on_close = false;
	bool on_open = false;
};

// ---------------------------------------------------------------------
// Helper struct to include a list of path nodes
// ---------------------------------------------------------------------
struct compare
{
	bool operator()(const PathNode* a, const PathNode* b)
	{
		return a->Score() >= b->Score();
	}
};

struct PathList
{
	// Looks for a node in this list and returns it's list node or NULL
	//p2List_item<PathNode>* Find(const iPoint& point) const; TODO - p2list
	std::list<PathNode>::iterator Find(const iPoint& point);

	// Returns the Pathnode with lowest score in this list or NULL if empty
	//p2List_item<PathNode>* GetNodeLowestScore() const;
	std::list<PathNode>::const_iterator GetNodeLowestScore() const;
	// -----------
	
	// The list itself, note they are not pointers!
	//std::list<PathNode> list;
	std::priority_queue<PathNode*, std::vector<PathNode*>, compare> list;
};



#endif // __j1PATHFINDING_H__