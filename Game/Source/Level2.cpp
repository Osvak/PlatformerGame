#include "Level2.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"

// Constructor
Level2::Level2(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	LOG("Creating Level2");

	name.Create("level2");


	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->entityManager = entityManager;
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
bool Level2::Update(float dt)
{
	//
	// Player Update
	//
	player->Update(dt);



	//
	// Scene controls
	//
	if (input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		map->drawColliders = !map->drawColliders;
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
	if (player->destroyed == true)
	{
		entityManager->DestroyEntity(player);
	}

	active = false;

	return true;
}