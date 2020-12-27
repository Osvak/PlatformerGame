#include "Level2.h"

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
#include "Potion.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"

// Constructor
Level2::Level2() : Module()
{
	name.Create("level2");
}

// Destructor
Level2::~Level2()
{}

// Called before render is available
bool Level2::Awake()
{
	LOG("Loading Scene 2");
	bool ret = true;

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

	return ret;
}

// Called before the first frame
bool Level2::Start()
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
	app->map->Load("map2.tmx");

	//
	// Load textures
	//
	cpTexture = app->tex->Load("Assets/Textures/Items/checkpoint_sheet.png");

	//
	// Load music
	//
	app->audio->PlayMusic("Assets/Audio/Music/map2_music.ogg");

	//
	// Move Camera to starting position
	//
	app->render->camera.x = -((int)app->win->GetScale() * TILE_SIZE);
	app->render->camera.y = -((int)app->win->GetScale() * TILE_SIZE * 14);

	// Checkpoint collider

	checkPointCollider = app->collisions->AddCollider({ TILE_SIZE * 44, TILE_SIZE * 20, 12, 20 }, Collider::ColliderType::CHECKPOINT, this);
	app->potion->potionCollider = app->collisions->AddCollider({ app->potion->potionPosition.x, app->potion->potionPosition.y, 8, 10 }, Collider::ColliderType::POTION, this);

	// Set savedPos to the start of the level 2
	app->player->savedPos.x = TILE_SIZE * 3;
	app->player->savedPos.y = TILE_SIZE * 24;

	//
	// Set current animation
	//
	currentAnim = cpIdleAnim;


	return true;
}

// Called each loop iteration
bool Level2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Level2::Update(float dt)
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
	SDL_Rect cpRect;

	// Lifes HUD Draw
	lifesRect.x = app->player->cameraCollider->rect.x - (TILE_SIZE * 5);
	lifesRect.y = app->player->cameraCollider->rect.y - (TILE_SIZE * 4);


	for (int i = 0; i < app->player->lifes; i++)
	{
		app->render->DrawTexture(app->player->lifesTexture, lifesRect.x + 17 * i, lifesRect.y);
	}

	// Checkpoint Draw

	// Animation Update

	currentAnim->Update();

	cpRect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(app->level2->cpTexture, TILE_SIZE * 44, TILE_SIZE * 20 - 4, &cpRect);



	if (app->potion->isCollected == false)
	{
		app->render->DrawTexture(app->potion->potionTexture, app->potion->potionPosition.x, app->potion->potionPosition.y);
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
		app->fadeToBlack->Fade(this, (Module*)app->level1, 60.0f);
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
bool Level2::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


bool Level2::Cp2Activation()
{
	isCpActive = true;

	app->player->savedPos.x = TILE_SIZE * 44;
	app->player->savedPos.y = TILE_SIZE * 20;

	return true;
}

// Called before quitting
bool Level2::CleanUp()
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