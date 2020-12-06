#include "SceneTitle.h"

#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Input.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "SceneLogo.h"
#include "Player.h"

#include "Log.h"


// Constructor
SceneTitle::SceneTitle() : Module()
{
	name.Create("sceneTitle");
}

// Destructor
SceneTitle::~SceneTitle()
{

}


// Called before render is available
bool SceneTitle::Awake()
{
	LOG("Loading Title Screen");
	
	return true;
}

// Called before the first frame / when activated
bool SceneTitle::Start()
{
	app->currentScene = TITLE;

	//
	// Load map
	//
	app->map->Load("scene_title.tmx");
	pressEnterToStart = app->tex->Load("Assets/maps/press_enter_to_start.png");

	//
	// Activate modules
	//
	active = true;
	app->map->Start();

	//
	// Load music
	//
	app->audio->PlayMusic("Assets/audio/music/title_screen_music.ogg");

	//
	// Move camera
	//
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	//
	// Reset lifes counter
	//
	app->player->lifes = 3;

	return true;
}


// Called before the Update
bool SceneTitle::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneTitle::Update(float dt)
{

	return true;
}

// Called after Updates
bool SceneTitle::PostUpdate()
{
	bool ret = true;

	//
	// Draw map
	//
	app->map->Draw();

	++blinkCont;
	if (blinkCont >= BLINK_TIME)
	{
		app->render->DrawTexture(pressEnterToStart, 9 * TILE_SIZE, 11 * TILE_SIZE);

		if (blinkCont == (2 * BLINK_TIME))
		{
			blinkCont = 0;
		}
	}


	//
	// Scene controls
	//
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN ||
		app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
		app->input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, (Module*)app->scene, 60.0f);
		return true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, (Module*)app->scene, 60.0f);
		return true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, (Module*)app->scene2, 60.0f);
		return true;
	}
	
	return ret;
}

// Called before quitting, frees memory and controls active and inactive modules
bool SceneTitle::CleanUp()
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