#include "SceneLogo.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"

#include "Log.h"


// Constructor
SceneLogo::SceneLogo(Input* input, Render* render, Textures* tex, AudioManager* audioManager)
{
	LOG("Creating SceneLogo");

	name.Create("sceneLogo");


	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
}

// Destructor
SceneLogo::~SceneLogo()
{
}


// Called before the first frame
bool SceneLogo::Load()
{
	LOG("Loading SceneLogo");

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

	//
	// Reset flags
	//
	nextSceneCounter = 0;
	transition = false;


	return false;
}


// Called each loop iteration
bool SceneLogo::Update(float dt)
{
	//
	// Check sound
	//
	if (nextSceneCounter < NEXT_SCENE_TIME && transition == false)
	{
		++nextSceneCounter;
		if (nextSceneCounter == NEXT_SCENE_TIME / 3)
		{
			audioManager->PlayFX(logoFX);
		}
	}

	//
	// Handle key inputs
	//
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


	return false;
}


bool SceneLogo::Draw()
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
bool SceneLogo::Unload()
{
	if (!active)
	{
		return false;
	}

	LOG("Unloading Logo Screen");

	//entityManager->DestroyEntity(map);
	tex->UnLoad(img);
	audioManager->UnloadFX(logoFX);

	active = false;

	return false;
}