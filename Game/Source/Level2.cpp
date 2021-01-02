#include "Level2.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"

// Constructor
Level2::Level2()
{
	LOG("Loading Level2");

	name.Create("level2");
}

// Destructor
Level2::~Level2()
{
}


// Called before the first frame
bool Level2::Load(Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	map->Load("map2.tmx");

	//
	// Activate modules
	//
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	player->position = fPoint(TILE_SIZE * 10, TILE_SIZE * 31);

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
bool Level2::Update(Input* input, AudioManager* audioManager, float dt)
{
	//
	// Player Update
	//
	player->Update(dt);



	//
	// Scene controls
	//
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

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
		TransitionToScene(SceneType::LEVEL2);
		return true;
	}

	//
	// Scene change
	//
	if (player->isWinning == true)
	{
		TransitionToScene(SceneType::WIN);
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
bool Level2::Draw(Render* render)
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
bool Level2::Unload(Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	if (!active)
	{
		return true;
	}

	LOG("Freeing Level 2");

	entityManager->DestroyEntity(map);
	entityManager->DestroyEntity(player);


	active = false;

	return true;
}