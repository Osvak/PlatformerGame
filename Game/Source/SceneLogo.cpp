#include "SceneLogo.h"

#include "App.h" // erase
#include "Window.h" // erase

#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "EntityManager.h"

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
bool SceneLogo::Load(Textures* tex, EntityManager* entityManager, AudioManager* audioManager)
{
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
	logoFX = audioManager->LoadFX("Assets/Audio/FX/logo.wav");
	audioManager->musicList.Add(&logoFX);

	//
	// Move camera
	//
	//app->render->camera.x = app->render->camera.y = 0;
	// TODO: Fix camera starting position


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
			// TODO: Fix sound in update
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
		TransitionToScene(SceneType::TITLE);
		transition = true;
		nextSceneCounter = 0;
		return true;
	}

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL1);
		transition = true;
		nextSceneCounter = 0;
		return true;
	}
	if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
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
bool SceneLogo::Unload(Textures* tex, AudioManager* audioManager)
{
	if (!active)
	{
		return true;
	}

	LOG("Freeing Logo Screen");

	tex->UnLoad(img);
	audioManager->UnloadFX(logoFX);


	active = false;

	return true;
}