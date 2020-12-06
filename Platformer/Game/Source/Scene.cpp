#include "Scene.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Collisions.h"
#include "FadeToBlack.h"

#include "Log.h"


// Constructor
Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->currentScene = LEVEL1;

	//
	// Activate modules
	//
	active = true;
	app->player->Start();
	
	//
	// Load map
	//
	app->map->Load("map.tmx");
	app->map->Start();

	//
	// Load music
	//
	app->audio->PlayMusic("Assets/audio/music/map_1_music.ogg");

	//
	// Move Camera to starting position
	//
	app->render->camera.x = -((int)app->win->GetScale() * TILE_SIZE);
	app->render->camera.y = -((int)app->win->GetScale() * TILE_SIZE * 2);


	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
    //
	// Scene controls
	//
	if(app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		if (freeCamera == false)
		{
			freeCamera = true;
		}
		else
		{
			freeCamera = false;
		}
	}

	if (freeCamera == true)
	{

		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += 15;

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= 15;

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += 15;

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= 15;
	}

	// Draw Map
	app->map->Draw();


	if (app->player->isWinning == true)
	{
		app->fadeToBlack->Fade(this, (Module*)app->scene2, 60.0f);
	}

	if (app->player->isDying == true)
	{
		app->fadeToBlack->Fade(this, (Module*)app->sceneLose, 60.0f);
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, this, 60.0f);
		return true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, (Module*)app->scene2, 60.0f);
		return true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, this, 60.0f);
		return true;
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	if (!active)
	{
		return true;
	}

	app->collisions->CleanUp();
	app->map->CleanUp();
	app->player->CleanUp();


	active = false;

	return true;
}