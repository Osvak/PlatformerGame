#include "Level1.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "AudioManager.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Collisions.h"
#include "FadeToBlack.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"


// Constructor
Level1::Level1()
{
	LOG("Loading Level1");

	name.Create("level1");

	//
	// Animation pushbacks
	//
	cpIdleAnim->loop = true;
	cpIdleAnim->speed = 0.1f;
	cpActiveAnim->loop = true;
	cpActiveAnim->speed = 0.1f;

	for (int i = 0; i < 9; i++)
		cpIdleAnim->PushBack({ i * 12, 0, 12, 20 });

	for (int i = 0; i < 3; i++)
		cpActiveAnim->PushBack({ i * 12, 20, 12, 20 });
}

// Destructor
Level1::~Level1()
{
}


// Called before the first frame
bool Level1::Load(Textures* tex)
{
	//
	// Load map
	//
	app->map->Load("map1.tmx");

	//
	// Activate modules
	//
	app->player->Start();

	//
	// Load textures
	//
	cpTexture = tex->Load("Assets/Textures/Items/checkpoint_sheet.png");

	//
	// Load music
	//
	app->audioManager->PlayMusic("Assets/Audio/Music/map1_music.ogg");

	//
	// Move Camera to starting position
	//
	app->render->camera.x = -((int)app->win->GetScale() * TILE_SIZE);
	app->render->camera.y = -((int)app->win->GetScale() * TILE_SIZE * 2);

	// Checkpoint collider
	//checkPointCollider = app->collisions->AddCollider({ TILE_SIZE * 39, TILE_SIZE * 14, 12, 20 }, Collider::ColliderType::CHECKPOINT, this);

	//
	// Set current animation
	//
	currentAnim = cpIdleAnim;


	return true;
}

// Called each loop iteration
bool Level1::Update(Input* input, float dt)
{
	bool ret = false;

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = true;
	}

	//
	// Scene controls
	//
	if (input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();
	}
	if (input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
	}

	if (input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		app->player->position = app->player->checkpointPos;
		app->player->cameraCollider->SetPos(app->player->checkpointPos.x, app->player->checkpointPos.y - TILE_SIZE * 4);
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL1);
		return false;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL2);
		return false;
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL1);
		return false;
	}


	if (app->player->isWinning == true)
	{
		TransitionToScene(SceneType::LEVEL2);
	}

	if (app->player->isDying == true)
	{
		if (app->player->lifes <= 0)
		{
			TransitionToScene(SceneType::LOSE);
		}
	}

	if (isCpActive == true)
	{

		currentAnim = cpActiveAnim;
	}

	return false;
}

// Called each loop iteration
bool Level1::Draw(Render* render)
{
	// Draw Map
	app->map->Draw();

	//SDL_Rect cpRect = currentAnim->GetCurrentFrame();
	SDL_Rect lifesRect;
	SDL_Rect cpRect;

	// Lifes HUD Draw
	lifesRect.x = app->player->cameraCollider->rect.x - (TILE_SIZE * 5);
	lifesRect.y = app->player->cameraCollider->rect.y - (TILE_SIZE * 4);

	for (int i = 0; i < app->player->lifes; i++)
	{
		render->DrawTexture(app->player->lifesTexture, lifesRect.x + 17 * i, lifesRect.y);
	}

	// Checkpoint Draw

	// Animation Update

	currentAnim->Update();

	cpRect = currentAnim->GetCurrentFrame();

	// Draw Checkpoint
	//app->render->DrawTexture(app->level1->cpTexture, TILE_SIZE * 39, TILE_SIZE * 14 - 4, &cpRect);


	return false;
}


// Called before quitting
bool Level1::Unload()
{
	LOG("Freeing Level 1");

	if (!active)
	{
		return false;
	}

	
	app->tex->UnLoad(cpTexture);

	app->collisions->CleanUp();
	app->map->CleanUp();
	app->player->CleanUp();

	isCpActive = false;


	active = false;

	return false;
}


bool Level1::Cp1Activation()
{
	isCpActive = true;

	currentAnim = cpActiveAnim;

	app->player->checkpointPos.x = TILE_SIZE * 38;
	app->player->checkpointPos.y = TILE_SIZE * 14;

	//app->SaveGameRequest();

	return false;
}