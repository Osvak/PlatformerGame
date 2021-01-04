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
	void ResetPath(iPoint start);

	// Getters
	DynArray<iPoint>* GetLastPath();
	PQueue<iPoint>* GetFrontier();
	List<iPoint>* GetVisited();
	List<iPoint>* GetBreadcrumbs();

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;
	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;
	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;

	bool PropagateAStar(const iPoint& destination);
	void ComputePathAStar(const iPoint& origin, const iPoint& destination);

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
	PQueue<iPoint> frontier;
	List<iPoint> visited;
	List<iPoint> breadcrumbs;

	// Map walkability values [0..255]
	uchar* map;
	bool destinationFound = false;

	DynArray<iPoint> lastPath;
};

#endif // !__PATHFINDING_H__