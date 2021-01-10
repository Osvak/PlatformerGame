#include "SceneWin.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"

#include "Log.h"


// Constructor
SceneWin::SceneWin(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	LOG("Creating Win Screen");

	name.Create("sceneWin");


	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->entityManager = entityManager;
}

// Destructor
SceneWin::~SceneWin()
{
}



// Called before the first frame / when activated
bool SceneWin::Load()
{
	LOG("Loading Win Screen");

	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	map->Load("scene_win.tmx");

	//
	// Load music
	//
	victoryFX = audioManager->LoadFX("Assets/Audio/FX/victory.wav");
	audioManager->musicList.Add(&victoryFX);

	//
	// Move camera
	//
	render->camera.x = render->camera.y = 0;

	//
	// Flags reset
	//
	playFX = true;


	return false;
}


// Called each loop iteration
bool SceneWin::Update(float dt)
{
	if (playFX == true)
	{
		audioManager->PlayFX(victoryFX);
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
bool SceneWin::Draw()
{
	//
	// Draw map
	//
	map->Draw();

	
	return false;
}


// Called before quitting, frees memory and controls activa and inactive modules
bool SceneWin::Unload()
{
	if (!active)
	{
		return false;
	}

	LOG("Unloading Win Screen");

	entityManager->DestroyEntity(map);
	audioManager->UnloadFX(victoryFX);


	active = false;

	return false;
}