#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__

#include "Module.h"

#include "Point.h"
#include "DynArray.h"
#include "List.h"
#include "PQueue.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255



class PathFinding : public Module
{
public:

	// NOTE: Constructor is private to avoid new instances creation
	// Get unique instance of the class
	static PathFinding* GetInstance();
	// Destructor
	~PathFinding();

	// Called before quitting
	bool CleanUp();


	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);


	// Create a path from A to B
	int CreatePath(const iPoint& origin, const iPoint& destination);

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;
	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;
	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;
	
	DynArray<iPoint>* GetLastPath();


private:

	// Singleton instance
	static PathFinding* instance;
	// Private constructor, alternatively: PathFinding()=delete;
	PathFinding();
	// Declare the copy constructor and the assignment operator as 
	// private (or delete them explicitly) to prevent cloning your object
	PathFinding(const PathFinding&);
	PathFinding& operator=(const PathFinding&);

	

	// Size of the map
	uint width;
	uint height;

	// Map walkability values [0..255]
	uchar* map;

	DynArray<iPoint> lastPath;
};

// Forward declaration
struct PathList;

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------
struct PathNode
{
	int accumulatedCost;
	int heuristic;
	iPoint pos;
	const PathNode* parent; // needed to reconstruct the path in the end

	// Convenient constructors
	PathNode();
	PathNode(int accumulatedCost, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathList& list_to_fill) const;

	// Calculates this tile score
	int Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);
};

// ---------------------------------------------------------------------
// Helper struct to include a list of path nodes
// ---------------------------------------------------------------------
struct PathList
{
	// Looks for a node in this list and returns it's list node or NULL
	ListItem<PathNode>* Find(const iPoint& point) const;

	// Returns the Pathnode with lowest score in this list or NULL if empty
	ListItem<PathNode>* GetNodeLowestScore() const;

	// The list itself, note they are not pointers!
	List<PathNode> list;
};

#endif // !__PATHFINDING_H__