#include "SceneWin.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"

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
bool SceneWin::Load(Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	map->Load("scene_win.tmx");

	//
	// Load music
	//
	audioManager->StopMusic();
	victoryFX = audioManager->LoadFX("Assets/Audio/FX/victory.wav");
	audioManager->musicList.Add(&victoryFX);

	//
	// Move camera
	//
	render->camera.x = render->camera.y = 0;


	return false;
}


// Called each loop iteration
bool SceneWin::Update(Input* input, AudioManager* audioManager, float dt)
{
	if (playFX == true)
	{
		audioManager->PlayFX(victoryFX); // TODO: Fix audio in update
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
	map->Draw();

	
	return false;
}


// Called before quitting, frees memory and controls activa and inactive modules
bool SceneWin::Unload(Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	if (!active)
	{
		return false;
	}

	LOG("Freeing Win Screen");

	entityManager->DestroyEntity(map);
	audioManager->UnloadFX(victoryFX);


	active = false;

	return false;
}