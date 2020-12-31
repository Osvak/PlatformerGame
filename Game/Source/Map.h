#ifndef __MAP_H__
#define __MAP_H__

#include "Entity.h"
#include "Render.h"
#include "Textures.h"

#include "Point.h"
#include "List.h"
#include "PQueue.h"
#include "DynArray.h"

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"


struct SDL_Texture;

// Struct to hold information for a TileSet
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;

	SDL_Texture* texture;
	int	texWidth;
	int	texHeight;
	int	numTilesWidth;
	int	numTilesHeight;
	int	offsetX;
	int	offsetY;
	int tilecount;

	// Method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
};

// Enum for map type, just for convenience,
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// Generic structure to hold properties
struct Properties
{
	struct Property
	{
		SString name;
		int value;
	};
	
	~Properties()
	{
		ListItem<Property*>* listD;
		listD = propertyList.start;

		while (listD != nullptr)
		{
			RELEASE(listD->data);
			listD = listD->next;
		}

		propertyList.Clear();
	}

	// Method to ask for the value of a custom property
	int GetProperty(const char* name, int default_value = 0) const;

	List<Property*> propertyList;
};

// Struct for the map layer
struct MapLayer
{
	SString	name;
	int width;
	int height;
	uint* data;

	Properties properties;

	//float parralax;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		uint ret = data[(y * width) + x];
		return ret;
	}
};

// Struct needed to hold the information to Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	SDL_Color backgroundColor;
	MapTypes type;

	List<TileSet*> tilesets;

	List<MapLayer*> layers;
};

class Map : public Entity
{
public:

	// Constructor
    Map(Textures* texture);
    // Destructor
    virtual ~Map();


	// Creates the colliders off the map layers
	//bool CreateColliders();

    // Draws the map
    void Draw(Render* render);
	// Draw each layer of the map
	void DrawLayer(Render* render, int num, bool parallax);


    // Called before quitting
    bool CleanUp();


    // Load new map
    bool Load(const char* path);


	// Method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	// Add orthographic world to map coordinates
	iPoint WorldToMap(int x, int y) const;


	SDL_Rect GetTilemapRec(int x, int y) const;

private:

	// Methods to load all required map data
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);

	// Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	// Pick the right Tileset based on a tile id
	TileSet* GetTilesetFromTileId(int id) const;

public:

    // Add your struct for map info
	MapData data;

	bool drawColliders = false;

	int cameraMaxBottomPosition;

private:

	Textures* tex;

    pugi::xml_document mapFile;
    SString folder;
    bool mapLoaded;

	uint32 scale;
	iPoint camOffset;
};

#endif // __MAP_H__