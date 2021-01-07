#include "PathFinding.h"

#include "App.h"

#include "Defs.h"
#include "Log.h"


PathFinding* PathFinding::instance = nullptr;

// Instance creator
PathFinding* PathFinding::GetInstance()
{
	if (instance == nullptr)
	{
		LOG("Creating PathFinding Instance");
		instance = new PathFinding();
	}
	else
	{
		//LOG("Getting PathFinding Instance");
	}

	return instance;
}
// Constructor
PathFinding::PathFinding() : map(NULL), width(0), height(0)
{
	name.Create("pathFinding");
}
// Destructor
PathFinding::~PathFinding()
{
	RELEASE_ARRAY(map);
	RELEASE(instance);
}

// Called before quitting
bool PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	lastPath.Clear();
	RELEASE_ARRAY(map);

	return true;
}


// Sets up the walkability map
void PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}


// Utility: return true if pos is inside the map boundaries
bool PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}
// Utility: returns true is the tile is walkable
bool PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}
// Utility: return the walkability value of a tile
uchar PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
	{
		return map[(pos.y * width) + pos.x];
	}

	return INVALID_WALK_CODE;
}



// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
	ListItem<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}
// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
	ListItem<PathNode>* ret = NULL;
	int min = 65535;

	ListItem<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}



// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : accumulatedCost(-1), heuristic(-1), pos(-1, -1), parent(NULL)
{}
PathNode::PathNode(int accumulatedCost, int h, const iPoint& pos, const PathNode* parent) : accumulatedCost(accumulatedCost), heuristic(h), pos(pos), parent(parent)
{}
PathNode::PathNode(const PathNode& node) : accumulatedCost(node.accumulatedCost), heuristic(node.heuristic), pos(node.pos), parent(node.parent)
{}
// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& listToFill) const
{
	iPoint cell;
	uint before = listToFill.list.Count();
	PathFinding* instance = PathFinding::GetInstance();

	// north
	cell.Create(pos.x, pos.y + 1);
	if (instance->IsWalkable(cell))
	{
		listToFill.list.Add(PathNode(-1, -1, cell, this));
	}
	// south
	cell.Create(pos.x, pos.y - 1);
	if (instance->IsWalkable(cell))
	{
		listToFill.list.Add(PathNode(-1, -1, cell, this));
	}
	// east
	cell.Create(pos.x + 1, pos.y);
	if (instance->IsWalkable(cell))
	{
		listToFill.list.Add(PathNode(-1, -1, cell, this));
	}
	// west
	cell.Create(pos.x - 1, pos.y);
	if (instance->IsWalkable(cell))
	{
		listToFill.list.Add(PathNode(-1, -1, cell, this));
	}

	return listToFill.list.Count();
}
// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return accumulatedCost + heuristic;
}
// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	accumulatedCost = parent->accumulatedCost + 1;
	heuristic = pos.DistanceTo(destination);

	return accumulatedCost + heuristic;
}



// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	//Making sure origin and destination are walkable
	if (IsWalkable(destination) == false || IsWalkable(origin) == false) return -1;
	else
	{
		lastPath.Clear();


		PathList open;
		PathList close;

		//Adding the start node
		open.list.Add(PathNode(0, origin.DistanceManhattan(destination), origin, nullptr));

		while (open.list.Count() != 0)
		{
			close.list.Add(open.GetNodeLowestScore()->data);
			open.list.Del(open.GetNodeLowestScore());

			//If destination has been reached add last node and flip the list with the resulting path
			if (close.list.end->data.pos == destination)
			{
				for (ListItem<PathNode>* i = close.list.end; i->data.parent != nullptr; i = close.Find(i->data.parent->pos))
				{
					iPoint newPos = i->data.parent->pos;
					lastPath.PushBack(newPos);
				}
				lastPath.PushBack(destination);
				lastPath.Flip();
				return 0;
			}
			//If not, check neighbours and add the most convenient, taking care of the possibility that one may already be in the list
			else
			{
				PathList neighbours;
				close.list.end->data.FindWalkableAdjacents(neighbours);

				for (ListItem<PathNode>* j = neighbours.list.start; j != NULL; j = j->next)
				{
					if (close.Find(j->data.pos)) continue;
					else if (open.Find(j->data.pos))
					{
						PathNode tmp = open.Find(j->data.pos)->data;
						j->data.CalculateF(destination);

						if (tmp.accumulatedCost > j->data.accumulatedCost) tmp.parent = j->data.parent;
					}
					else
					{
						j->data.CalculateF(destination);
						open.list.Add(j->data);
					}
				}
				neighbours.list.Clear();
			}
		}
	}
	return -1;
}
// Returns the last path created
DynArray<iPoint>* PathFinding::GetLastPath()
{
	return &lastPath;
}