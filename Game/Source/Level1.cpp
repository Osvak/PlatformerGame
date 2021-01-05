#include "Level1.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"
#include "Pathfinding.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"


// Constructor
Level1::Level1(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager, PathFinding* pathFinding)
{
	LOG("Creating Level1");

	name.Create("level1");

	
	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->entityManager = entityManager;
	this->pathFinding = pathFinding;
}

// Destructor
Level1::~Level1()
{
}


// Called before the first frame
bool Level1::Load()
{
	LOG("Loading Level1");

	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	if (map->Load("map1.tmx") == true)
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
	// Load player
	//
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	player->position = fPoint((TILE_SIZE * 9), (TILE_SIZE * 17) - player->playerHeight);

	//
	// Add enemies
	//
	enemySkeleton = (EnemySkeleton*)entityManager->CreateEntity(EntityType::ENEMY_SKELETON);
	enemySkeleton->position = fPoint((TILE_SIZE) * 18, (TILE_SIZE * 16) - enemySkeleton->height);
	//enemySkeleton->position = fPoint(TILE_SIZE * 39, TILE_SIZE * 14);

	//
	// Load music
	//
	audioManager->PlayMusic("Assets/Audio/Music/map1_music.ogg");

	//
	// Move Camera to starting position
	//
	render->camera.x = -((int)render->scale * TILE_SIZE * 3);
	render->camera.y = -((int)render->scale * TILE_SIZE * 7);


	return false;
}


// Called each loop iteration
bool Level1::Update(float dt)
{
	//
	// Player Update
	//
	player->Update(dt);

	//
	// Enemies Update
	//
	enemySkeleton->Update(dt, player->position, map);

	//
	// Collision check
	//
	if (player->godMode == false)
	{
		CollisionLogic();
	}

	//
	// Scene controls
	//
	if (input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		//app->SaveGameRequest();
	}
	if (input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		//app->LoadGameRequest();
	}
	if (input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		//player->position = player->checkpointPos;
		//player->cameraCollider->SetPos(player->checkpointPos.x, player->checkpointPos.y - TILE_SIZE * 4);
	}
	if (input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		map->drawColliders = !map->drawColliders;
	}

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL1);
		return true;
	}
	if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL2);
		return true;
	}
	if (input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL1);
		return true;
	}

	//
	// Scene change
	//
	if (player->isWinning == true)
	{
		TransitionToScene(SceneType::LEVEL2);
	}

	if (player->isDying == true)
	{
		if (player->lifes <= 0)
		{
			TransitionToScene(SceneType::LOSE);
		}
	}


	return true;
}

// Called each loop iteration
bool Level1::Draw()
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
	enemySkeleton->Draw();

	//
	// Draw Colliders
	//
	if (map->drawColliders == true)
	{
		map->DrawColliders();

		Uint8 alpha = 100;
		render->DrawRectangle(player->GetRect() , 0, 0, 255, alpha);
		render->DrawRectangle(enemySkeleton->GetRect(), 255, 0, 0, alpha);
	}


	return false;
}


// Called before quitting
bool Level1::Unload()
{
	if (!active)
	{
		return false;
	}

	LOG("Freeing Level 1");

	entityManager->DestroyEntity(map);
	entityManager->DestroyEntity(enemySkeleton);
	if (player->destroyed == true)
	{
		entityManager->DestroyEntity(player);
	}

	active = false;

	return false;
}


inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}



void Level1::CollisionLogic()
{
	MapLayer* layer;
	TileSet* tileset;
	iPoint coords;
	SDL_Rect tileRect;
	SDL_Rect colliderRect;
	fPoint tempPlayerPosition = player->position;
	
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
					if (tileId == 1)
					{
						tileRect = tileset->GetTileRect(tileId);
						colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
						if(CheckCollision(colliderRect, player->GetRect()) == true)
						{
							player->isTouchingGround = true;
						}
					}
					if (tileId == 2)
					{
						tileRect = tileset->GetTileRect(tileId);
						colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
						if (CheckCollision(colliderRect, player->GetRect()) == true)
						{
							player->isDying = true;
							--player->lifes;
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
					if (tileId == 4)
					{
						tileRect = tileset->GetTileRect(tileId);
						colliderRect = { coords.x, coords.y, tileRect.w, tileRect.h };
						if (CheckCollision(colliderRect, player->GetRect()) == true)
						{
							player->isTouchingWall = true;
						}
					}
				}
			}
		}
	}
}