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

#include "SDL/include/SDL_rect.h"


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

	//
	// Animation pushbacks
	//
	cpIdleAnim->loop = true;
	cpIdleAnim->speed = 0.2f;
	cpActiveAnim->loop = true;
	cpActiveAnim->speed = 0.1f;
	
	for (int i = 0; i < 9; i++)
		cpIdleAnim->PushBack({ 0,0,12,20 });

	for (int i = 0; i < 3; i++)
		cpActiveAnim->PushBack({ 0,20,12,20 });


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
	// Load textures
	//
	cpTexture = app->tex->Load("Assets\textures\items\checkpoint_sheet.png");

	//
	// Load music
	//
	app->audio->PlayMusic("Assets/audio/music/map_1_music.ogg");

	//
	// Move Camera to starting position
	//
	app->render->camera.x = -((int)app->win->GetScale() * TILE_SIZE);
	app->render->camera.y = -((int)app->win->GetScale() * TILE_SIZE * 2);

	// Checkpoint Collider

	checkPointCollider = app->collisions->AddCollider({ TILE_SIZE * 38, TILE_SIZE * 14, 12, 20 }, Collider::ColliderType::CHECKPOINT, this);

	currentAnim = cpIdleAnim;


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

	SDL_Rect cpRect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(app->scene->cpTexture, TILE_SIZE * 38, TILE_SIZE * 14, &cpRect);


	if (app->player->isWinning == true)
	{
		app->fadeToBlack->Fade(this, (Module*)app->scene2, 60.0f);
	}

	if (app->player->isDying == true)
	{
	if (app->player->lifes <= 0)
		{
			app->fadeToBlack->Fade(this, (Module*)app->sceneLose, 60.0f);
		}
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

	

	if (isCpActive == true)
	{

		currentAnim = cpActiveAnim;
	}

	//
	// Animation Update
	//
	currentAnim->Update();

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

	isCpActive = false;


	active = false;

	return true;
}

bool Scene::Cp1Activation()
{
	isCpActive = true;

	app->player->savedPos.x = TILE_SIZE * 38;
	app->player->savedPos.y = TILE_SIZE * 14;

	//app->SaveGameRequest();

	return true;
}