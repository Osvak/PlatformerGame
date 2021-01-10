#include "Level2.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"
#include "Pathfinding.h"

#include "Map.h"
#include "Player.h"
#include "EnemySkeleton.h"
#include "EnemyGhost.h"
#include "ItemPotion.h"
#include "ItemCoin.h"
#include "Checkpoint.h"

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
	// Add Checkpoints
	//
	checkpoint1 = (Checkpoint*)entityManager->CreateEntity(EntityType::CHECKPOINT);
	checkpoint1->position = fPoint(TILE_SIZE * 38, TILE_SIZE * 28 - checkpoint1->height);
	checkpoint2 = (Checkpoint*)entityManager->CreateEntity(EntityType::CHECKPOINT);
	checkpoint2->position = fPoint(TILE_SIZE * 43, TILE_SIZE * 17 - checkpoint2->height);

	//
	// Add player
	//
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	player->InitPositions(fPoint(TILE_SIZE * 10, TILE_SIZE * 32 - player->height));
	player->LoadPlayer();

	//
	// Add enemies
	//
	enemySkeleton1 = (EnemySkeleton*)entityManager->CreateEntity(EntityType::ENEMY_SKELETON);
	enemySkeleton1->position = fPoint(TILE_SIZE * 21, TILE_SIZE * 32 - enemySkeleton1->height);
	enemySkeleton2 = (EnemySkeleton*)entityManager->CreateEntity(EntityType::ENEMY_SKELETON);
	enemySkeleton2->position = fPoint(TILE_SIZE * 55, TILE_SIZE * 28 - enemySkeleton2->height);
	enemySkeleton3 = (EnemySkeleton*)entityManager->CreateEntity(EntityType::ENEMY_SKELETON);
	enemySkeleton3->position = fPoint(TILE_SIZE * 43, TILE_SIZE * 17 - enemySkeleton3->height);
	enemyGhost1 = (EnemyGhost*)entityManager->CreateEntity(EntityType::ENEMY_GHOST);
	enemyGhost1->position = fPoint(TILE_SIZE * 35, TILE_SIZE * 13 - enemyGhost1->height);
	enemyGhost2 = (EnemyGhost*)entityManager->CreateEntity(EntityType::ENEMY_GHOST);
	enemyGhost2->position = fPoint(TILE_SIZE * 23, TILE_SIZE * 10 - enemyGhost2->height);

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
	// Checkpoint Update
	//
	checkpoint1->Update(dt, player);
	checkpoint2->Update(dt, player);
	CheckpointLogic();

	//
	// Enemies Update
	//
	enemySkeleton1->Update(dt, player, map);
	enemySkeleton2->Update(dt, player, map);
	enemySkeleton3->Update(dt, player, map);
	enemyGhost1->Update(dt, player, map);
	enemyGhost2->Update(dt, player, map);

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
	if (drawColliders == true)
	{
		map->drawColliders = true;
	}
	else
	{
		map->drawColliders = false;
	}
	if (playerGodMode == true)
	{
		player->godMode = true;
	}
	else
	{
		player->godMode = false;
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
	// Draw Checkpoints
	//
	checkpoint1->Draw();
	checkpoint2->Draw();

	//
	// Draw Items
	//
	itemPotion->Draw();

	//
	// Draw Player
	//
	player->Draw();

	//
	// Draw Enemies
	//
	enemySkeleton1->Draw();
	enemySkeleton2->Draw();
	enemySkeleton3->Draw();
	enemyGhost1->Draw();
	enemyGhost2->Draw();
	

	//
	// Draw Colliders
	//
	if (map->drawColliders == true)
	{
		map->DrawColliders();
		checkpoint1->DrawColliders();
		checkpoint2->DrawColliders();
		player->DrawColliders();
		enemySkeleton1->DrawColliders();
		enemySkeleton2->DrawColliders();
		enemySkeleton3->DrawColliders();
		enemyGhost1->DrawColliders();
		enemyGhost2->DrawColliders();
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
	entityManager->DestroyEntity(checkpoint1);
	entityManager->DestroyEntity(checkpoint2);
	entityManager->DestroyEntity(enemySkeleton1);
	entityManager->DestroyEntity(enemySkeleton2);
	entityManager->DestroyEntity(enemySkeleton3);
	entityManager->DestroyEntity(enemyGhost1);
	entityManager->DestroyEntity(enemyGhost2);
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

void Level2::CheckpointLogic()
{
	// Control when a checkpoint is activated
	if (checkpoint1->isActivated == true)
	{
		player->savedPos.x = checkpoint1->position.x;
		player->savedPos.y = checkpoint1->position.y - 8;
	}
	if (checkpoint2->isActivated == true)
	{
		player->savedPos.x = checkpoint2->position.x;
		player->savedPos.y = checkpoint2->position.y - 8;
	}

	// Control when you start travelling
	if (checkpoint1->playerNearby == true && input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		checkpoint1->travelling = !checkpoint1->travelling;
		checkpoint2->travelling = !checkpoint2->travelling;
		player->isTravelling = !player->isTravelling;
		player->position.x = checkpoint1->position.x;
		player->position.y = checkpoint1->position.y - 8;
	}
	if (checkpoint2->playerNearby == true && input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		checkpoint1->travelling = !checkpoint1->travelling;
		checkpoint2->travelling = !checkpoint2->travelling;
		player->isTravelling = !player->isTravelling;
		player->position.x = checkpoint2->position.x;
		player->position.y = checkpoint2->position.y - 8;
	}

	// Control travelling mechanic
	if (player->position.x == checkpoint1->position.x && player->isTravelling && checkpoint2->isActivated)
	{
		if (input->GetKey(SDL_SCANCODE_D) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			player->position.x = checkpoint2->position.x;
			player->position.y = checkpoint2->position.y - 8;
			player->cameraRect.x = (int)player->position.x;
			player->cameraRect.y = (int)player->position.y - (TILE_SIZE * 3);
			player->cameraPosition.x = player->position.x;
			player->cameraPosition.y = player->position.y + player->height - player->cameraRect.h;
		}
	}
	if (player->position.x == checkpoint2->position.x && player->isTravelling && checkpoint2->isActivated)
	{
		if (input->GetKey(SDL_SCANCODE_A) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			player->position.x = checkpoint1->position.x;
			player->position.y = checkpoint1->position.y - 8;
			player->cameraRect.x = (int)player->position.x;
			player->cameraRect.y = (int)player->position.y - (TILE_SIZE * 3);
		}
	}
}