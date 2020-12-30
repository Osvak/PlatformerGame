#include "Level2.h"

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
#include "Potion.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"

// Constructor
Level2::Level2()
{
	LOG("Loading Scene 2");

	name.Create("level2");


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
Level2::~Level2()
{
}


// Called before the first frame
bool Level2::Load(Textures* tex)
{
	//
	// Load map
	//
	app->map->Load("map2.tmx");

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
	app->audioManager->PlayMusic("Assets/Audio/Music/map2_music.ogg");

	//
	// Move Camera to starting position
	//
	app->render->camera.x = -((int)app->win->GetScale() * TILE_SIZE * 2);
	app->render->camera.y = -((int)app->win->GetScale() * TILE_SIZE * 21);

	// Checkpoint collider
	//checkPointCollider = app->collisions->AddCollider({ TILE_SIZE * 44, TILE_SIZE * 20, 12, 20 }, Collider::ColliderType::CHECKPOINT, this);
	//app->potion->potionCollider = app->collisions->AddCollider({ app->potion->potionPosition.x, app->potion->potionPosition.y, 8, 10 }, Collider::ColliderType::POTION, this);

	// Set savedPos to the start of the level 2
	app->player->checkpointPos.x = TILE_SIZE * 10;
	app->player->checkpointPos.y = TILE_SIZE * 31;

	//
	// Set current animation
	//
	currentAnim = cpIdleAnim;


	return true;
}

// Called each loop iteration
bool Level2::Update(Input* input, float dt)
{
	//
	// Scene controls
	//
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

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

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL1);
		return true;
	}
	if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL2);
		return true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL2);
		return true;
	}

	if (app->potion->isCollected == false)
	{
		app->render->DrawTexture(app->potion->potionTexture, app->potion->potionPosition.x, app->potion->potionPosition.y);
	}

	if (app->player->isWinning == true)
	{
		TransitionToScene(SceneType::WIN);
	}

	if (app->player->isDying == true)
	{
		if (app->player->lifes <= 0)
		{
			TransitionToScene(SceneType::LOSE);
		}
	}

	return true;
}

// Called each loop iteration
bool Level2::Draw(Render* render)
{
	// Draw Map
	app->map->Draw();

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

	//app->render->DrawTexture(app->level2->cpTexture, TILE_SIZE * 44, TILE_SIZE * 20 - 4, &cpRect);


	return false;
}


// Called before quitting
bool Level2::Unload()
{
	LOG("Freeing Level 2");

	if (!active)
	{
		return true;
	}

	app->tex->UnLoad(cpTexture);

	app->collisions->CleanUp();
	app->map->CleanUp();
	app->player->CleanUp();

	isCpActive = false;

	active = false;

	return true;
}


bool Level2::Cp2Activation()
{
	isCpActive = true;

	app->player->checkpointPos.x = TILE_SIZE * 44;
	app->player->checkpointPos.y = TILE_SIZE * 20;

	return true;
}