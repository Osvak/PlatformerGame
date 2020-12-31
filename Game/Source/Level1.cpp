#include "Level1.h"

#include "EntityManager.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"


// Constructor
Level1::Level1()
{
	LOG("Loading Level1");

	name.Create("level1");
}

// Destructor
Level1::~Level1()
{
}


// Called before the first frame
bool Level1::Load(Textures* tex, EntityManager* entityManager, AudioManager* audioManager)
{
	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	map->Load("map1.tmx");

	//
	// Load player
	//
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	player->position = fPoint(TILE_SIZE * 9, TILE_SIZE * 16);

	//
	// Load music
	//
	audioManager->PlayMusic("Assets/Audio/Music/map1_music.ogg");

	//
	// Move Camera to starting position
	//
	//app->render->camera.x = -((int)app->win->GetScale() * TILE_SIZE);
	//app->render->camera.y = -((int)app->win->GetScale() * TILE_SIZE * 2);
	// TODO: Fix camera starting position


	return true;
}

// Called each loop iteration
bool Level1::Update(Input* input, float dt)
{
	//
	// Player Update
	//
	player->Update(dt);



	//
	// Scene controls
	//
	bool ret = false;

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = true;
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
		player->position = player->checkpointPos;
		//app->player->cameraCollider->SetPos(app->player->checkpointPos.x, app->player->checkpointPos.y - TILE_SIZE * 4);
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

	//
	// Scene change
	//
	if (player->isWinning == true)
	{
		TransitionToScene(SceneType::LEVEL2);
	}

	if (player->isDying == true)
	{
		if (player->lifes <= 0)
		{
			TransitionToScene(SceneType::LOSE);
		}
	}


	return false;
}

// Called each loop iteration
bool Level1::Draw(Render* render)
{
	//
	// Draw Map
	//
	map->Draw(render);

	//
	// Draw Player
	//
	player->Draw(render);

	//SDL_Rect cpRect = currentAnim->GetCurrentFrame();
	SDL_Rect lifesRect;
	SDL_Rect cpRect;

	// Lifes HUD Draw
	//lifesRect.x = app->player->cameraCollider->rect.x - (TILE_SIZE * 5);
	//lifesRect.y = app->player->cameraCollider->rect.y - (TILE_SIZE * 4);

	/*for (int i = 0; i < app->player->lifes; i++)
	{
		render->DrawTexture(app->player->lifesTexture, lifesRect.x + 17 * i, lifesRect.y);
	}*/

	// Draw Checkpoint
	//app->render->DrawTexture(app->level1->cpTexture, TILE_SIZE * 39, TILE_SIZE * 14 - 4, &cpRect);


	return false;
}


// Called before quitting
bool Level1::Unload(Textures* tex, AudioManager* audioManager)
{
	if (!active)
	{
		return false;
	}

	LOG("Freeing Level 1");

	map->CleanUp();
	player->CleanUp(tex, audioManager);


	active = false;

	return false;
}