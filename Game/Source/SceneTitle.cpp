#include "SceneTitle.h"

#include "App.h"
#include "AudioManager.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "SceneLogo.h"
#include "Player.h"


#include "Input.h"
#include "Render.h"
#include "Textures.h"

//#include "EntityManager.h"

#include "Log.h"


// Constructor
SceneTitle::SceneTitle()
{
	LOG("Loading Title Screen");

	name.Create("sceneTitle");
}

// Destructor
SceneTitle::~SceneTitle()
{
}



// Called before the first frame / when activated
bool SceneTitle::Load(Textures* tex)
{
	//
	// Load map
	//
	app->map->Load("scene_title.tmx");
	pressEnterToStart = tex->Load("Assets/Maps/press_enter_to_start.png");

	//
	// Load music
	//
	app->audioManager->PlayMusic("Assets/Audio/Music/title_screen_music.ogg");

	//
	// Move camera
	//
	app->render->camera.x = app->render->camera.y = 0;



	return false;
}



// Called each loop iteration
bool SceneTitle::Update(Input* input, float dt)
{
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
		TransitionToScene(SceneType::LEVEL1);
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


bool SceneTitle::Draw(Render* render)
{
	//
	// Draw map
	//
	app->map->Draw();

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
		return true;
	}

	LOG("Freeing Title Sreen");

	app->map->CleanUp();

	app->tex->UnLoad(pressEnterToStart);


	active = false;

	return true;
}