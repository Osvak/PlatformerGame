#include "SceneLose.h"

#include "App.h"
#include "Audio.h"
#include "Render.h"
#include "Input.h"
#include "Map.h"
#include "FadeToBlack.h"

#include "Log.h"

 // Constructor
SceneLose::SceneLose() : Module()
{
	name.Create("sceneLose");
}

 // Destructor
SceneLose::~SceneLose()
{

}

// Called before render is available
bool SceneLose::Awake()
{
	LOG("Loading Lose Screen");

	return true;
}

 // Called before the first frame / when activated
bool SceneLose::Start()
{
	
	app->currentScene = LOSE;

	//
	// Load map
	//
	app->map->Load("scene_lose.tmx");

	//
	// Activate modules
	//
	active = true;
	app->map->Start();

	//
	// Load music
	//

	//
	// Move camera
	//
	app->render->camera.x = 0;
	app->render->camera.y = 0;


	return true;
}

// Called before the Update
bool SceneLose::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLose::Update(float dt)
{
	return true;
}

 // Called after Updates
bool SceneLose::PostUpdate()
{
	bool ret = true;

	//
	// Draw map
	//
	app->map->Draw();

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
		app->fadeToBlack->Fade(this, (Module*)app->sceneTitle, 60.0f);
		return true;
	}

	return ret;
}

// Called before quitting, frees memory and controls activa and inactive modules
bool SceneLose::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Freeing Lose Screen");

	app->map->CleanUp();
	active = false;

	return true;
}