#include "Map.h"

#include "Textures.h"
#include "Window.h"
#include "Render.h"

#include "PQueue.h"
#include "DynArray.h"
#include "Defs.h"
#include "Log.h"

#include "PugiXml\src\pugixml.hpp"

#include <math.h>
#include <string.h>


// Ask for the value of a custom property
int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* item = propertyList.start;

	while (item != NULL)
	{
		if (item->data->name == value)
		{
			return item->data->value;
		}

		item = item->next;
	}

	return defaultValue;
}



// Constructor of the Map
Map::Map(Textures* tex, Window* win, Render* render) : Entity(EntityType::MAP)
{
	LOG("Creating Map Entity");

	mapLoaded = false;
	name.Create("map");
	folder.Create("Assets/Maps/");

	this->tex = tex;
	this->win = win;
	this->render = render;
	scale = win->GetScale();
}
// Destructor
Map::~Map()
{
}



// Called before the first frame
bool Map::DrawColliders()
{
	MapLayer* layer;
	TileSet* tileset;
	iPoint coords;

	SDL_Rect tileRect;
	SDL_Rect colliderRect;

	Uint8 alpha = 100;

	for (ListItem<MapLayer*>* item = data.layers.start; item; item = item->next)
	{
		layer = item->data;

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tileId = layer->Get(x, y);
				coords = MapToWorld(x, y);

				if (tileId > 0)
				{
					tileset = GetTilesetFromTileId(tileId);
					if (tileId == 1)
					{
						tileRect = tileset->GetTileRect(tileId);
						colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
						render->DrawRectangle(colliderRect, 0, 230, 185, alpha); // turquoise
					}
					if (tileId == 2)
					{
						tileRect = tileset->GetTileRect(tileId);
						colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
						render->DrawRectangle(colliderRect, 255, 0, 0, alpha); // red
					}
					if (tileId == 3)
					{
						tileRect = tileset->GetTileRect(tileId);
						colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
						render->DrawRectangle(colliderRect, 0, 0, 255, alpha); // blue
					}
					if (tileId == 4)
					{
						tileRect = tileset->GetTileRect(tileId);
						colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
						render->DrawRectangle(colliderRect, 105, 85, 157, alpha); // mauve
					}
				}
			}
		}
	}

	return true;
}
// Draw the map
void Map::Draw()
{
	if (mapLoaded == false) return;

	camOffset.x = render->camera.x;
	camOffset.y = render->camera.y;

	for (unsigned int i = 0; i < data.layers.Count(); i++)
	{
		if (data.layers[i]->properties.GetProperty("visible", 1) != 0)
		{
			if (data.layers[i]->properties.GetProperty("parallax", 1) != 0)
			{
				DrawLayer(i, true);
			}
			else
			{
				DrawLayer(i, false);
			}
		}
	}
}
// Draw each layer of the map
void Map::DrawLayer(int num, bool parallax)
{
	if (num < data.layers.Count())
	{
		MapLayer* layer = data.layers[num];

		//render->scale = scale;

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tileId = layer->Get(x, y);

				if (tileId > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tileId);

					SDL_Rect rec = tileset->GetTileRect(tileId);
					iPoint pos = MapToWorld(x, y);

					if (parallax == true)
					{
						render->DrawTexture(tileset->texture, pos.x + tileset->offsetX, pos.y + tileset->offsetY, &rec, 1.3f);
					}
					else
					{
						render->DrawTexture(tileset->texture, pos.x + tileset->offsetX, pos.y + tileset->offsetY, &rec);
					}
				}
			}
		}

		//render->scale = 1; // ???
	}
}


// Method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	// Add isometric map to world coordinates
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth / 2);
		ret.y = (x + y) * (data.tileHeight / 2);
	}
	else
	{
	LOG("Unknown map type");
	ret.x = x; ret.y = y;
	}

	return ret;
}
// Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	// Add the case for isometric maps to WorldToMap
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tileWidth * 0.5f;
		float half_height = data.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect Map::GetTilemapRec(int x, int y) const
{
	iPoint pos = MapToWorld(x, y);
	SDL_Rect rec = { pos.x * scale + camOffset.x, pos.y * scale + camOffset.y,
		data.tileWidth * scale, data.tileHeight * scale };

	return rec;
}
// Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}
	
	return set;
}
// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));
	
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading Map");


    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.Clear();


	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.Clear();


	// Clean up the pugui tree
	mapFile.reset();


    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
	LOG("Loading map with name: %s", filename);

	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}


	// Load general info
	if (ret == true)
	{
		ret = LoadMap();
	}


	// Load tilesets info
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);
		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.Add(set);
	}

	ret = true;


	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true) data.layers.Add(lay);
	}


	//CreateColliders(); // RIP old Colliders


	cameraMaxBottomPosition = (data.height * TILE_SIZE) - (TILE_SIZE * 12);


	if (ret == true)
	{
		// LOG all the data loaded iterate all tilesets and LOG everything
		LOG("Succesfully load map XML file: %s", filename);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tileWidth, data.tileHeight);

		LOG("--- Tileset Info:");
		int count = data.tilesets.Count();

		for (int i = 0; i < count; ++i)
		{
			LOG("name: %s	firstGid: %d", data.tilesets.At(i)->data->name.GetString(), data.tilesets.At(i)->data->firstgid);
			LOG("tile width: %d tile height: %d", data.tilesets.At(i)->data->tileWidth, data.tilesets.At(i)->data->tileHeight);
			LOG("spacing: %d margin: %d", data.tilesets.At(i)->data->spacing, data.tilesets.At(i)->data->margin);

		}


		// LOG the info for each loaded layer
		LOG("--- Layer Info:");
		for (int i = 0; i < count; ++i)
		{
			LOG("name: %s", data.tilesets.At(i)->data->name.GetString());
			LOG("tile width: %d tile height: %d", data.tilesets.At(i)->data->tileWidth, data.tilesets.At(i)->data->tileHeight);
		}
	}

	mapLoaded = ret;

	if (mapLoaded == true)
	{
		LOG("Map Loaded Correctly");
	}

    return ret;
}
// Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// Load map general properties

		data.height = map.attribute("height").as_int();
		data.width = map.attribute("width").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();


		SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
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
// Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// L03: DONE: Load Tileset attributes
	set->name.Create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		//set->offsetX = offset.attribute("x").as_int();
		//set->offsetY = offset.attribute("y").as_int();
	}
	else
	{
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}
// Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// Load Tileset image
		const char* path = PATH(folder.GetString(), image.attribute("source").as_string());
		set->texture = tex->Load(path);
		set->texHeight = image.attribute("height").as_int();
		set->texWidth = image.attribute("width").as_int();
		set->numTilesHeight = set->texHeight / set->tileHeight;
		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->offsetX = 0;
		set->offsetY = 0;

		LOG("Tileset image loaded correctly");
	}

	return ret;
}
// Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	pugi::xml_node layerData = node.child("data");

	if (layerData == NULL)
	{
		LOG("Error with XML file.");

		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width * layer->height];
		memset(layer->data, 0, layer->width * layer->height);

		pugi::xml_node tile = layerData.child("tile");
		for (int i = 0; tile; ++i)
		{
			layer->data[i] = tile.attribute("gid").as_uint(0);
			tile = tile.next_sibling("tile");
		}
	}

	// Load layer properties
	LoadProperties(node, layer->properties);

	return ret;
}
// Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node property;

		for (property = data.child("property"); property; property = property.next_sibling("property"))
		{
			Properties::Property* newProperty = new Properties::Property();

			newProperty->name = property.attribute("name").as_string();
			newProperty->value = property.attribute("value").as_int();

			properties.propertyList.Add(newProperty);
		}
	}

	return ret;
}


// Create walkability map for pathfinding
bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item;
	item = data.layers.start;

	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.GetProperty("navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tileId = layer->Get(x, y);
				TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tileId) > 0 ? 0 : 1;
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