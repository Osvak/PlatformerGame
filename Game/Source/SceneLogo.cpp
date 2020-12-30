#include "SceneLogo.h"

#include "App.h"
#include "Window.h"
#include "AudioManager.h"
#include "Map.h"
#include "FadeToBlack.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Log.h"


// Constructor
SceneLogo::SceneLogo()
{
	LOG("Loading SceneLogo");

	name.Create("sceneLogo");

	//
	// Reset flags
	//
	nextSceneCounter = 0;
	transition = false;

}

// Destructor
SceneLogo::~SceneLogo()
{
}


// Called before the first frame
bool SceneLogo::Load(Textures* tex)
{
	//app->currentScene = LOGO;

	//
	// Load Map
	//
	img = tex->Load("Assets/Maps/scene_logo.png");
	imgW = (int)app->win->GetWidth() / (int)app->win->GetScale();
	imgH = (int)app->win->GetHeight() / (int)app->win->GetScale();

	if (SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH) != 0)
	{
		LOG("Error on SceneLogo QueryTexture");
	}

	//
	// Load music
	//
	logoFX = app->audioManager->LoadFX("Assets/Audio/FX/logo.wav");
	app->musicList.Add(&logoFX);

	//
	// Move camera
	//
	app->render->camera.x = app->render->camera.y = 0;


	return true;
}


// Called each loop iteration
bool SceneLogo::Update(Input* input, float dt)
{
	//
	// Check sound
	//
	if (nextSceneCounter < NEXT_SCENE_TIME && transition == false)
	{
		++nextSceneCounter;
		if (nextSceneCounter == 15)
		{
			app->audioManager->PlayFX(logoFX);
		}
	}

	//
	// Handle key inputs
	//
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

	if (input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN ||
		nextSceneCounter == NEXT_SCENE_TIME)
	{
		//app->fadeToBlack->Fade(this, (Module*)app->sceneTitle, 60.0f); // remove
		TransitionToScene(SceneType::TITLE);
		transition = true;
		nextSceneCounter = 0;
		return true;
	}

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		//app->fadeToBlack->Fade(this, (Module*)app->level1, 60.0f); // remove
		TransitionToScene(SceneType::LEVEL1);
		transition = true;
		nextSceneCounter = 0;
		return true;
	}
	if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		//app->fadeToBlack->Fade(this, (Module*)app->level2, 60.0f); // remove
		TransitionToScene(SceneType::LEVEL2);
		transition = true;
		nextSceneCounter = 0;
		return true;
	}

	return true;
}


bool SceneLogo::Draw(Render* render)
{
	//
	// Draw map
	//
	if (render->DrawTexture(img, 0, 0) == false)
	{
		LOG("Error drawing Scene Logo");
	}

	return true;
}

// Called before quitting, frees memory
bool SceneLogo::Unload()
{
	if (!active)
	{
		return true;
	}

	LOG("Freeing Logo Screen");

	app->tex->UnLoad(img);

	app->audioManager->UnloadFX(logoFX);

	app->map->CleanUp();
	active = false;

	return true;
}