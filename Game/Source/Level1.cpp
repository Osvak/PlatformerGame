#include "Level1.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"


// Constructor
Level1::Level1()
{
	LOG("Loading Level1");

	name.Create("level1");
}

// Destructor
Level1::~Level1()
{
}


// Called before the first frame
bool Level1::Load(Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	map->Load("map1.tmx");

	//
	// Load player
	//
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	player->position = fPoint(TILE_SIZE * 9, TILE_SIZE * 16);

	//
	// Load music
	//
	audioManager->PlayMusic("Assets/Audio/Music/map1_music.ogg");

	//
	// Move Camera to starting position
	//
	render->camera.x = -((int)render->scale * TILE_SIZE);
	render->camera.y = -((int)render->scale * TILE_SIZE * 2);


	return false;
}

// Called each loop iteration
bool Level1::Update(Input* input, AudioManager* audioManager, float dt)
{
	//
	// Player Update
	//
	player->Update(dt);



	//
	// Scene controls
	//
	bool ret = true;

	/*if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}*/
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
bool Level1::Draw(Render* render)
{
	//
	// Draw Map
	//
	map->Draw();

	//
	// Draw Player
	//
	player->Draw();
	

	return false;
}


// Called before quitting
bool Level1::Unload(Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	if (!active)
	{
		return false;
	}

	LOG("Freeing Level 1");

	entityManager->DestroyEntity(map);
	entityManager->DestroyEntity(player);


	active = false;

	return false;
}