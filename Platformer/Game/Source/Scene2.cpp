#include "Scene2.h"

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
Scene2::Scene2() : Module()
{
	name.Create("scene2");
}

// Destructor
Scene2::~Scene2()
{}

// Called before render is available
bool Scene2::Awake()
{
	LOG("Loading Scene 2");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene2::Start()
{
	app->currentScene = LEVEL2;

	//
	// Activate modules
	//
	active = true;
	app->player->Start();

	//
	// Load map
	//
	app->map->Load("map_2.tmx");
	app->map->Start();

	//
	// Load textures
	//

	//
	// Load music
	//
	app->audio->PlayMusic("Assets/audio/music/map_2_music.ogg");

	//
	// Move Camera to starting position
	//
	app->render->camera.x = -((int)app->win->GetScale() * TILE_SIZE);
	app->render->camera.y = -((int)app->win->GetScale() * TILE_SIZE * 14);

	// Checkpoint collider

	checkPointCollider = app->collisions->AddCollider({ TILE_SIZE * 44, TILE_SIZE * 20, 12, 20 }, Collider::ColliderType::CHECKPOINT, this);

	// Set savedPos to the start of the level 2
	app->player->savedPos.x = TILE_SIZE * 3;
	app->player->savedPos.y = TILE_SIZE * 24;

	return true;
}

// Called each loop iteration
bool Scene2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene2::Update(float dt)
{
	//
	// Scene controls
	//
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		app->player->position = app->player->savedPos;
		app->player->cameraCollider->SetPos(app->player->savedPos.x, app->player->savedPos.y - TILE_SIZE * 4);
	}

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

	SDL_Rect lifesRect;
	lifesRect.x = app->player->cameraCollider->rect.x - (TILE_SIZE * 5);
	lifesRect.y = app->player->cameraCollider->rect.y - (TILE_SIZE * 4);

	for (int i = 0; i < app->player->lifes; i++)
	{
		app->render->DrawTexture(app->player->lifesTexture, lifesRect.x + 17 * i, lifesRect.y);
	}

	if (app->player->isWinning == true)
	{
		app->fadeToBlack->Fade(this, (Module*)app->sceneWin, 60.0f);
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
		app->fadeToBlack->Fade(this, (Module*)app->scene, 60.0f);
		return true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, this, 60.0f);
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
bool Scene2::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene2::CleanUp()
{
	LOG("Freeing Scene 2");

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

bool Scene2::Cp2Activation()
{
	isCpActive = true;

	app->player->savedPos.x = TILE_SIZE * 44;
	app->player->savedPos.y = TILE_SIZE * 20;

	return true;
}