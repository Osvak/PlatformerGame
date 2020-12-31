#include "SceneLose.h"

#include "Input.h"

#include "EntityManager.h"

#include "Log.h"

 // Constructor
SceneLose::SceneLose()
{
	LOG("Loading Lose Screen");

	name.Create("sceneLose");
}

 // Destructor
SceneLose::~SceneLose()
{
}


 // Called before the first frame / when activated
bool SceneLose::Load(Textures* tex, EntityManager* entityManager, AudioManager* audioManager)
{
	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	map->Load("scene_lose.tmx");

	//
	// Load music
	//
	audioManager->StopMusic();
	gameOverFX = audioManager->LoadFX("Assets/Audio/FX/game_over.wav");
	audioManager->musicList.Add(&gameOverFX);

	//
	// Move camera
	//
	//app->render->camera.x = app->render->camera.y = 0;
	// TODO: Fix camera starting position

	return false;
}

// Called each loop iteration
bool SceneLose::Update(Input* input, float dt)
{
	if (playFX == true)
	{
		app->audioManager->PlayFX(gameOverFX); // TODO: Fix audio in update
		playFX = false;
	}


	//
	// Scene controls
	//
	bool ret = false;

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = true;
	}
	if (input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN)
	{
		TransitionToScene(SceneType::TITLE);
		return false;
	}

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL1);
		return false;
	}
	if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL2);
		return false;
	}

	return true;
}

 // Called after Updates
bool SceneLose::Draw(Render* render)
{
	//
	// Draw map
	//
	map->Draw(render);


	return false;
}


// Called before quitting, frees memory and controls activa and inactive modules
bool SceneLose::Unload(Textures* tex, AudioManager* audioManager)
{
	if (!active)
	{
		return false;
	}

	LOG("Freeing Lose Screen");

	map->CleanUp();
	audioManager->UnloadFX(gameOverFX);


	active = false;

	return false;
}