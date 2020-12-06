#include "SceneWin.h"

#include "App.h"
#include "Audio.h"
#include "Render.h"
#include "Input.h"
#include "Map.h"
#include "FadeToBlack.h"

#include "Log.h"


// Constructor
SceneWin::SceneWin() : Module()
{
	name.Create("sceneWin");
}

// Destructor
SceneWin::~SceneWin()
{

}


// Called before render is available
bool SceneWin::Awake()
{
	LOG("Loading Win Screen");

	return true;
}

// Called before the first frame / when activated
bool SceneWin::Start()
{
	app->currentScene = WIN;

	//
	// Load map
	//
	app->map->Load("scene_win.tmx");

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
bool SceneWin::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneWin::Update(float dt)
{
	return true;
}

// Called after Updates
bool SceneWin::PostUpdate()
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

// Called before quitting, frees memory and controls activa and inactive modules
bool SceneWin::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Freeing Win Screen");

	app->map->CleanUp();
	active = false;

	return true;
}