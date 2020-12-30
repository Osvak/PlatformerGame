#include "SceneWin.h"

#include "App.h"
#include "AudioManager.h"
#include "Render.h"
#include "Input.h"
#include "Map.h"

#include "Log.h"


// Constructor
SceneWin::SceneWin()
{
	LOG("Loading Win Screen");

	name.Create("sceneWin");


	//
	// Flags reset
	//
	playFX = true;

}

// Destructor
SceneWin::~SceneWin()
{
}



// Called before the first frame / when activated
bool SceneWin::Load(Textures* tex)
{
	//
	// Load map
	//
	app->map->Load("scene_win.tmx");

	//
	// Load music
	//
	app->audioManager->StopMusic();
	victoryFX = app->audioManager->LoadFX("Assets/Audio/FX/victory.wav");
	app->musicList.Add(&victoryFX);

	//
	// Move camera
	//
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return false;
}


// Called each loop iteration
bool SceneWin::Update(Input* input, float dt)
{
	if (playFX == true)
	{
		app->audioManager->PlayFX(victoryFX);
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

	return false;
}

// Called after Updates
bool SceneWin::Draw(Render* render)
{
	//
	// Draw map
	//
	app->map->Draw();

	
	return false;
}

// Called before quitting, frees memory and controls activa and inactive modules
bool SceneWin::Unload()
{
	if (!active)
	{
		return false;
	}

	LOG("Freeing Win Screen");

	app->map->CleanUp();

	app->audioManager->UnloadFX(victoryFX);

	active = false;

	return false;
}