#include "Level1.h"

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
	// Add Checkpoints
	//
	checkpoint1 = (Checkpoint*)entityManager->CreateEntity(EntityType::CHECKPOINT);
	checkpoint1->position = fPoint(TILE_SIZE * 17, TILE_SIZE * 16 - checkpoint1->height);
	checkpoint2 = (Checkpoint*)entityManager->CreateEntity(EntityType::CHECKPOINT);
	checkpoint2->position = fPoint(TILE_SIZE * 55, TILE_SIZE * 11 - checkpoint2->height);

	//
	// Add player
	//
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	player->InitPositions(fPoint(TILE_SIZE * 9, TILE_SIZE * 17 - player->height));
	if (player->destroyed == true)
	{
		player->LoadPlayer();
	}

	//
	// Add enemies
	//
	enemySkeleton = (EnemySkeleton*)entityManager->CreateEntity(EntityType::ENEMY_SKELETON);
	enemySkeleton->position = fPoint(TILE_SIZE * 39, TILE_SIZE * 15 - enemySkeleton->height);
	enemyGhost = (EnemyGhost*)entityManager->CreateEntity(EntityType::ENEMY_GHOST);
	enemyGhost->position = fPoint(TILE_SIZE * 65, TILE_SIZE * 10 - enemyGhost->height);

	//
	// Add items
	//
	itemPotion = (ItemPotion*)entityManager->CreateEntity(EntityType::ITEM_POTION);
	itemPotion->position = fPoint(TILE_SIZE * 65 + 4, TILE_SIZE * 13 - itemPotion->height);
	itemCoin = (ItemCoin*)entityManager->CreateEntity(EntityType::ITEM_COIN);
	itemCoin->position = fPoint(TILE_SIZE * 17, TILE_SIZE * 15 - itemCoin->height);

	

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
	enemySkeleton->Update(dt, player, map);
	enemyGhost->Update(dt, player, map);
	

	//
	// Items Update
	//
	itemPotion->Update(dt, player);
	itemCoin->Update(dt, player);


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
		TransitionToScene(SceneType::LEVEL2);
	}
	if (player->destroyed == true)
	{
		TransitionToScene(SceneType::LOSE);
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
	// Draw Checkpoints
	//
	checkpoint1->Draw();
	checkpoint2->Draw();

	//
	// Draw Items
	//
	itemPotion->Draw();
	itemCoin->Draw();

	//
	// Draw Player
	//
	player->Draw();
	
	//
	// Draw Enemies
	//
	enemySkeleton->Draw();
	enemyGhost->Draw();

	

	//
	// Draw Colliders
	//
	if (map->drawColliders == true)
	{
		map->DrawColliders();
		checkpoint1->DrawColliders();
		checkpoint2->DrawColliders();
		player->DrawColliders();
		enemySkeleton->DrawColliders();
		enemyGhost->DrawColliders();
		itemPotion->DrawColliders();
		itemCoin->DrawColliders();
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

	LOG("Unloading Level 1");

	entityManager->DestroyEntity(map);
	entityManager->DestroyEntity(checkpoint1);
	entityManager->DestroyEntity(checkpoint2);
	entityManager->DestroyEntity(enemySkeleton);
	entityManager->DestroyEntity(enemyGhost);
	entityManager->DestroyEntity(itemPotion);
	entityManager->DestroyEntity(itemCoin);
	if (player->destroyed == true)
	{
		entityManager->DestroyEntity(player);
		player->ResetInstance();
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

void Level1::CheckpointLogic()
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