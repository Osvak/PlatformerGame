#include "SceneLogo.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
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
bool SceneLogo::Load(Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	//
	// Load Map
	//
	//map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	//map->Load("scene_logo.tmx");
	img = tex->Load("Assets/Maps/scene_logo.png");
	imgW = (int)render->camera.w / (int)render->scale;
	imgH = (int)render->camera.h / (int)render->scale;
	if (SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH) != 0)
	{
		LOG("Error on QueryTexture");
	}

	//
	// Load music
	//
	logoFX = audioManager->LoadFX("Assets/Audio/FX/logo.wav");
	audioManager->musicList.Add(&logoFX);

	//
	// Move camera
	//
	render->camera.x = render->camera.y = 0;


	return false;
}


// Called each loop iteration
bool SceneLogo::Update(Input* input, AudioManager* audioManager, float dt)
{
	//
	// Check sound
	//
	if (nextSceneCounter < NEXT_SCENE_TIME && transition == false)
	{
		++nextSceneCounter;
		if (nextSceneCounter == 200)
		{
			audioManager->PlayFX(logoFX);
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

	return false;
}


bool SceneLogo::Draw(Render* render)
{
	//
	// Draw map
	//
	//map->Draw();
	if (render->DrawTexture(img, 0, 0) == false)
	{
		LOG("Error drawing Scene Logo");
	}

	return false;
}

// Called before quitting, frees memory
bool SceneLogo::Unload(Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	if (!active)
	{
		return false;
	}

	LOG("Freeing Logo Screen");

	//entityManager->DestroyEntity(map);
	tex->UnLoad(img);

	active = false;

	return false;
}