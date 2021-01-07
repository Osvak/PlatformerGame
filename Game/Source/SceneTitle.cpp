#include "SceneTitle.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"

#include "Log.h"


// Constructor
SceneTitle::SceneTitle(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	LOG("Creating Title Screen");

	name.Create("sceneTitle");


	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->entityManager = entityManager;
}

// Destructor
SceneTitle::~SceneTitle()
{
}



// Called before the first frame / when activated
bool SceneTitle::Load()
{
	LOG("Loading Title Screen");

	//
	// Load map
	//
	Textures* p = tex;
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	map->Load("scene_title.tmx");
	pressEnterToStart = tex->Load("Assets/Maps/press_enter_to_start.png");

	//
	// Load music
	//
	audioManager->PlayMusic("Assets/Audio/Music/title_screen_music.ogg");

	//
	// Move camera
	//
	render->camera.x = render->camera.y = 0;

	return false;
}



// Called each loop iteration
bool SceneTitle::Update(float dt)
{
	//
	// Scene controls
	//
	if (input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL1);
		return false;
	}


	return false;
}


bool SceneTitle::Draw()
{
	//
	// Draw map
	//
	map->Draw();

	++blinkCont;
	if (blinkCont >= BLINK_TIME)
	{
		render->DrawTexture(pressEnterToStart, 9 * TILE_SIZE, 11 * TILE_SIZE);

		if (blinkCont == (2 * BLINK_TIME))
		{
			blinkCont = 0;
		}
	}

	return false;
}


// Called before quitting, frees memory and controls active and inactive modules
bool SceneTitle::Unload()
{
	if (!active)
	{
		return false;
	}

	LOG("Unloading Title Sreen");

	tex->UnLoad(pressEnterToStart);
	entityManager->DestroyEntity(map);


	active = false;

	return false;
}