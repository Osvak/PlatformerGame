#include "Level2.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"
#include "Pathfinding.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"

// Constructor
Level2::Level2(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager, PathFinding* pathFinding)
{
	LOG("Creating Level2");

	name.Create("level2");


	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->entityManager = entityManager;
	this->pathFinding = pathFinding;
}
// Destructor
Level2::~Level2()
{
}


// Called before the first frame
bool Level2::Load()
{
	LOG("Loading Level2");

	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	if (map->Load("map2.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (map->CreateWalkabilityMap(w, h, &data))
		{
			pathFinding->SetMap(w, h, data);
		}

		RELEASE_ARRAY(data);
	}

	//
	// Add player
	//
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	player->InitPositions(fPoint(TILE_SIZE * 10, TILE_SIZE * 32 - player->height));
	player->LoadPlayer();

	//
	// Add enemies
	//
	// TODO: Level 2 enemies

	//
	// Add items
	//
	itemPotion = (ItemPotion*)entityManager->CreateEntity(EntityType::ITEM_POTION);
	itemPotion->position = fPoint(TILE_SIZE * 57 + 4, TILE_SIZE * 23 - itemPotion->height);


	//
	// Load music
	//
	audioManager->PlayMusic("Assets/Audio/Music/map2_music.ogg");

	//
	// Move Camera to starting position
	//
	render->camera.x = -((int)render->scale * TILE_SIZE * 2);
	render->camera.y = -((int)render->scale * TILE_SIZE * 21);


	return true;
}

// Called each loop iteration
bool Level2::Update(float dt)
{
	//
	// Player Update
	//
	player->Update(dt, map);
	if (freeCamera == true)
	{
		player->freeCamera = true;
	}
	else
	{
		player->freeCamera = false;
	}

	//
	// Enemies Update
	//
	// TODO: Level 2 enemies

	//
	// Items Update
	//
	itemPotion->Update(dt, player);


	//
	// Collision check between player and map (death and win)
	//
	if (player->godMode == false)
	{
		CollisionLogic();
	}



	//
	// Scene controls
	//
	if (input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		map->drawColliders = !map->drawColliders;
	}
	if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		player->godMode = !player->godMode;
	}

	//
	// Scene change
	//
	if (player->changeLevel == true)
	{
		player->changeLevel = false;
		TransitionToScene(SceneType::WIN);
	}
	if (player->destroyed == true)
	{
		TransitionToScene(SceneType::LOSE);
	}


	return true;
}

// Called each loop iteration
bool Level2::Draw()
{
	//
	// Draw Map
	//
	map->Draw();

	//
	// Draw Player
	//
	player->Draw();

	//
	// Draw Enemies
	//


	//
	// Draw Items
	//
	itemPotion->Draw();

	//
	// Draw Colliders
	//
	if (map->drawColliders == true)
	{
		map->DrawColliders();
		player->DrawColliders();
		itemPotion->DrawColliders();
	}


	return false;
}


// Called before quitting
bool Level2::Unload()
{
	if (!active)
	{
		return true;
	}

	LOG("Unloading Level 2");

	entityManager->DestroyEntity(map);
	entityManager->DestroyEntity(itemPotion);
	if (player->destroyed == true)
	{
		entityManager->DestroyEntity(player);
		player->ResetInstance();
	}

	active = false;

	return true;
}


inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}
void Level2::CollisionLogic()
{
	MapLayer* layer;
	TileSet* tileset;
	iPoint coords;
	SDL_Rect tileRect;
	SDL_Rect colliderRect;

	for (ListItem<MapLayer*>* item = map->data.layers.start; item; item = item->next)
	{
		layer = item->data;

		for (int y = 0; y < map->data.height; ++y)
		{
			for (int x = 0; x < map->data.width; ++x)
			{
				int tileId = layer->Get(x, y);
				coords = map->MapToWorld(x, y);

				if (tileId > 0)
				{
					tileset = map->GetTilesetFromTileId(tileId);

					if (tileId == 2)
					{
						tileRect = tileset->GetTileRect(tileId);
						colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
						if (CheckCollision(colliderRect, player->GetRect()) == true)
						{
							player->isDying = true;
						}
					}
					if (tileId == 3)
					{
						tileRect = tileset->GetTileRect(tileId);
						colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
						if (CheckCollision(colliderRect, player->GetRect()) == true)
						{
							player->isWinning = true;
						}
					}
				}
			}
		}
	}
}