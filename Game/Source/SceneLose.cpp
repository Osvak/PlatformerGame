#include "SceneLose.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"

#include "Log.h"

 // Constructor
SceneLose::SceneLose(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	LOG("Creating Lose Screen");

	name.Create("sceneLose");


	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->entityManager = entityManager;
}

 // Destructor
SceneLose::~SceneLose()
{
}


 // Called before the first frame / when activated
bool SceneLose::Load()
{
	LOG("Loading Lose Screen");

	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	map->Load("scene_lose.tmx");

	//
	// Load music
	//
	gameOverFX = audioManager->LoadFX("Assets/Audio/FX/game_over.wav");
	audioManager->musicList.Add(&gameOverFX);

	//
	// Move camera
	//
	render->camera.x = render->camera.y = 0;

	return false;
}

// Called each loop iteration
bool SceneLose::Update(float dt)
{
	if (playFX == true)
	{
		audioManager->PlayFX(gameOverFX);
		playFX = false;
	}


	//
	// Scene controls
	//
	if (input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		TransitionToScene(SceneType::TITLE);
		return false;
	}


	return false;
}

 // Called after Updates
bool SceneLose::Draw()
{
	//
	// Draw map
	//
	map->Draw();


	return false;
}


// Called before quitting, frees memory and controls activa and inactive modules
bool SceneLose::Unload()
{
	if (!active)
	{
		return false;
	}

	LOG("Unloading Lose Screen");

	entityManager->DestroyEntity(map);
	audioManager->UnloadFX(gameOverFX);


	active = false;

	return false;
}