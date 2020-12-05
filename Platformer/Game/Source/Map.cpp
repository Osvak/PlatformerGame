#include "Map.h"

#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Collisions.h"

#include "PQueue.h"
#include "DynArray.h"
#include "Defs.h"
#include "Log.h"

#include "PugiXml\src\pugixml.hpp"

#include <math.h>


// Constructor
Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Ask for the value of a custom property
int Properties::GetProperty(const char* value, int defaultValue) const
{
	for (int i = 0; i < propertyList.Count(); i++)
	{
		if (strcmp(propertyList.At(i)->data->name.GetString(), value) == 0) // Checks the name of the property
		{
			if (propertyList.At(i)->data->value != defaultValue) // Checks if the value the property has is different than what we gave
			{
				return propertyList.At(i)->data->value; // Returns the new value
			}
			else 
			{
				return defaultValue;
			}
		}
	}

	return defaultValue;
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

// Called before the first frame
bool Map::Start()
{
	MapLayer* layer;
	TileSet* tileset;
	iPoint coords;

	SDL_Rect tileRect;
	SDL_Rect colliderRect;

	for (ListItem<MapLayer*>* item = app->map->data.layers.start; item; item = item->next)
	{
		layer = item->data;

		for (int y = 0; y < app->map->data.height; ++y)
		{
			for (int x = 0; x < app->map->data.width; ++x)
			{
				int tileId = layer->Get(x, y);
				coords = app->map->MapToWorld(x, y);
				tileset = app->map->GetTilesetFromTileId(tileId);

				if (tileId == 310)
				{
					tileRect = tileset->GetTileRect(tileId);
					colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
					app->collisions->AddCollider(colliderRect, Collider::ColliderType::WALL, this);
				}
				if (tileId == 307)
				{
					tileRect = tileset->GetTileRect(tileId);
					colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
					app->collisions->AddCollider(colliderRect, Collider::ColliderType::PLATFORM, this);
				}
				if (tileId == 308)
				{
					tileRect = tileset->GetTileRect(tileId);
					colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
					app->collisions->AddCollider(colliderRect, Collider::ColliderType::DIE, this);
				}
				if (tileId == 309)
				{
					tileRect = tileset->GetTileRect(tileId);
					colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
					app->collisions->AddCollider(colliderRect, Collider::ColliderType::NEXT_LEVEL, this);
				}
			}
		}
	}


	active = true;

	return true;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	MapLayer* layer;
	TileSet* tileset;
	iPoint coord;


	// L06: TODO 4: Make sure we draw all the layers and not just the first one
	for (ListItem<MapLayer*>* item = data.layers.start; item; item = item->next)
	{
		layer = item->data;

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tileId = layer->Get(x, y);
				coord = MapToWorld(x, y);
				tileset = GetTilesetFromTileId(tileId);

				if (tileId > 0)
				{
					// Complete the draw function
					for (int i = 0; i < data.tilesets.Count(); ++i)
					{
						if ((data.layers.At(i)->data->properties.GetProperty("visible", true)) == true)
						{
							SDL_Rect rect = data.tilesets.At(i)->data->GetTileRect(tileId);
							app->render->DrawTexture(tileset->texture, coord.x, coord.y, &rect);
						}
					}
				}
			}
		}
	}
}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	// L05: DONE 1: Add isometric map to world coordinates
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

// Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set;

	if (app->currentScene == TITLE)
	{
		if (id < 171) // Checks for first tileset (tileset)
		{

		}
		else if (id >= 171 && id < 226) // Checks for second tileset (background)
		{
			item = item->next;
		}
		else if (id >= 226) // Checks for third tileset (collider)
		{
			item = item->next;
			item = item->next;
		}
	}

	if (app->currentScene == LEVEL1)
	{
		if (id < 137) // Checks for first tileset (tileset)
		{

		}
		else if (id >= 137 && id < 307) // Checks for second tileset (background)
		{
			item = item->next;
		}
		else if (id >= 307) // Checks for third tileset (collider / walls)
		{
			item = item->next;
			item = item->next;
		}
	}

	if (app->currentScene == WIN)
	{
		if (id < 171)
		{

		}
		else if (id >= 171 && id < 307)
		{
			item = item->next;
		}
		else if (id >= 307)
		{
			item = item->next;
			item = item->next;
		}
	}

	if (app->currentScene == LOSE)
	{
		if (id < 171)
		{

		}
		else if (id >= 171 && id < 307)
		{
			item = item->next;
		}
		else if (id >= 307)
		{
			item = item->next;
			item = item->next;
		}
	}


	set = item->data;

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
    LOG("Unloading map");


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


	active = false;

    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
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
	pugi::xml_node layerNode;
	for (layerNode = mapFile.child("map").child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layerNode, lay);

		if (ret == true)
		{
			//ret = LoadLayer(layerNode, lay);
			data.layers.Add(lay);
		}

		//data.layers.Add(lay);

		pugi::xml_node propertiesNode;
		for (propertiesNode = layerNode.child("properties"); propertiesNode && ret; propertiesNode = propertiesNode.next_sibling("properties"))
		{
			Properties* property = new Properties();

			ret = LoadProperties(propertiesNode, *property);

			lay->properties = *property;
		}
	}

	if (ret == true)
	{
		// LOG all the data loaded iterate all tilesets and LOG everything
		LOG("Succesfully parsed map XML file: %s", filename);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tileWidth, data.tileHeight);

		LOG("Tileset ----");
		int count = data.tilesets.Count();

		for (int i = 0; i < count; ++i)
		{
			LOG("name: %s	firstGid: %d", data.tilesets.At(i)->data->name.GetString(), data.tilesets.At(i)->data->firstgid);
			LOG("tile width: %d tile height: %d", data.tilesets.At(i)->data->tileWidth, data.tilesets.At(i)->data->tileHeight);
			LOG("spacing: %d margin: %d", data.tilesets.At(i)->data->spacing, data.tilesets.At(i)->data->margin);

		}


		// LOG the info for each loaded layer
		LOG("Layer ----");
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
	
	// Load Tileset attributes

	set->name = tileset_node.attribute("name").as_string();
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tilecount = tileset_node.attribute("tilecount").as_int();

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

		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
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
	
	// Load a single layer
	// Header data
	layer->name = node.attribute("name").as_string("");
	layer->width = node.attribute("width").as_int(0);
	layer->height = node.attribute("height").as_int(0);

	// Tile array for the layer
	uint* pointer = new uint[layer->width * layer->height];
	memset(pointer, 0, layer->width * layer->height);
	layer->data = pointer;
	
	if (layer->data == 0)
	{
		LOG("Array creation error");
		ret = false;
	}
	else
	{
		pugi::xml_node tileGidNode = node.child("data").child("tile");

		for (int i = 0; tileGidNode && ret; tileGidNode = tileGidNode.next_sibling("tile"), ++i)
		{
			layer->data[i] = tileGidNode.attribute("gid").as_uint(0);
		}
	}

	return ret;
}


// Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;

	pugi::xml_node propertyNode = node.child("property");


	for (propertyNode; propertyNode && ret; propertyNode = propertyNode.next_sibling("property"))
	{
		Properties::Property* propertyID = new Properties::Property();
		propertyID->name = propertyNode.attribute("name").as_string("");

		Properties::Property* propertyType = new Properties::Property();
		propertyType->name = propertyNode.attribute("type").as_string("");


		if (propertyType->name == "int")
		{
			propertyID->value = propertyNode.attribute("value").as_int(0);
		}
		if (propertyType->name == "bool")
		{
			propertyID->value = propertyNode.attribute("value").as_bool(false);
		}
		properties.propertyList.Add(propertyID);
	}
	return ret;
}