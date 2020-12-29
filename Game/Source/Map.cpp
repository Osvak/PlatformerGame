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

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

// Called before the first frame
bool Map::CreateColliders()
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

				if (tileId > 0)
				{
					tileset = app->map->GetTilesetFromTileId(tileId);

					if (app->currentScene == LEVEL1 || app->currentScene == LEVEL2)
					{
						if (tileId == 1)
						{
							tileRect = tileset->GetTileRect(tileId);
							colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
							app->collisions->AddCollider(colliderRect, Collider::ColliderType::PLATFORM, this);
						}
						if (tileId == 2)
						{
							tileRect = tileset->GetTileRect(tileId);
							colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
							app->collisions->AddCollider(colliderRect, Collider::ColliderType::DIE, this);
						}
						if (tileId == 3)
						{
							tileRect = tileset->GetTileRect(tileId);
							colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
							app->collisions->AddCollider(colliderRect, Collider::ColliderType::NEXT_LEVEL, this);
						}
						if (tileId == 4)
						{
							tileRect = tileset->GetTileRect(tileId);
							colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
							app->collisions->AddCollider(colliderRect, Collider::ColliderType::WALL, this);
						}
					}
				}
			}
		}
	}


	active = true;

	return true;
}

// Draw the map
void Map::Draw()
{
	if (mapLoaded == false) return;

	for (int i = 0; i < data.layers.Count(); i++)
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
						app->render->DrawTexture(tileset->texture, pos.x + tileset->offsetX, pos.y + tileset->offsetY, &rec, 1.3f);
					}
					else
					{
						app->render->DrawTexture(tileset->texture, pos.x + tileset->offsetX, pos.y + tileset->offsetY, &rec);
					}
				}
			}
		}
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

// Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	/*if (app->currentScene == TITLE)
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
		if (id < 5) // Checks for first tileset (collider)
		{

		}
		else if (id >= 5 && id < 141) // Checks for second tileset (tileset)
		{
			item = item->next;
		}
		else if (id >= 141) // Checks for third tileset (background)
		{
			item = item->next;
			item = item->next;
		}
	}

	if (app->currentScene == LEVEL2)
	{
		if (id < 5) // Checks for first tileset (collider)
		{

		}
		else if (id >= 5 && id < 141) // Checks for second tileset (tileset)
		{
			item = item->next;
		}
		else if (id >= 141 && id < 205) // Checks for third tileset (clouds)
		{
			item = item->next;
			item = item->next;
		}
		else if (id >= 205) // Checks for fourth tileset (background)
		{
			item = item->next;
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
	
	set = item->data;*/

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
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true) data.layers.Add(lay);
	}


	CreateColliders();


	cameraMaxBottomPosition = (app->map->data.height * TILE_SIZE) - (TILE_SIZE * 12);


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
			layer->data[i] = tile.attribute("gid").as_int(0);
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