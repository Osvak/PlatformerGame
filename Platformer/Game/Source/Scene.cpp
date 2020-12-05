#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

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
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	//
	// Move Camera to starting position
	//
	app->render->camera.x = -((int)app->win->GetScale() * TILE_SIZE);
	app->render->camera.y = 0;


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

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 5;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 5;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 5;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 5;


	// Draw Map
	app->map->Draw();


	// Window Title update
	//SString title("Platformer Game, Map:%dx%d Tiles:%dx%d Tilesets:%d",
	//			   app->map->data.width, app->map->data.height,
	//			   app->map->data.tileWidth, app->map->data.tileHeight,
	//			   app->map->data.tilesets.Count());
	//
	//app->win->SetTitle(title.GetString());

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

	app->map->CleanUp();
	app->collisions->CleanUp();
	app->player->CleanUp();

	active = false;

	return true;
}
