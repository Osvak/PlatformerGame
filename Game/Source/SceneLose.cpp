#include "SceneLose.h"

#include "App.h"
#include "AudioManager.h"
#include "Render.h"
#include "Input.h"
#include "Map.h"
#include "FadeToBlack.h"

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
bool SceneLose::Load(Textures* tex)
{
	//
	// Load map
	//
	app->map->Load("scene_lose.tmx");

	//
	// Load music
	//
	app->audioManager->StopMusic();
	gameOverFX = app->audioManager->LoadFX("Assets/Audio/FX/game_over.wav");
	app->musicList.Add(&gameOverFX);

	//
	// Move camera
	//
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return false;
}

// Called each loop iteration
bool SceneLose::Update(Input* input, float dt)
{
	if (playFX == true)
	{
		app->audioManager->PlayFX(gameOverFX);
		playFX = false;
	}


	//
	// Scene controls
	//
	bool ret = false;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN ||
		app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
		app->input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN)
	{
		TransitionToScene(SceneType::TITLE);
		return false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL1);
		return false;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
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
	app->map->Draw();


	return false;
}


// Called before quitting, frees memory and controls activa and inactive modules
bool SceneLose::Unload()
{
	if (!active)
	{
		return false;
	}

	LOG("Freeing Lose Screen");

	app->map->CleanUp();

	app->audioManager->UnloadFX(gameOverFX);

	active = false;

	return false;
}