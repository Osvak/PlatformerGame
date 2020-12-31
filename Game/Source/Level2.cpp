#include "Level2.h"

#include "EntityManager.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"

// Constructor
Level2::Level2()
{
	LOG("Loading Level2");

	name.Create("level2");
}

// Destructor
Level2::~Level2()
{
}


// Called before the first frame
bool Level2::Load(Textures* tex, EntityManager* entityManager, AudioManager* audioManager)
{
	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	map->Load("map2.tmx");

	//
	// Activate modules
	//
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	player->position = fPoint(TILE_SIZE * 10, TILE_SIZE * 31);

	//
	// Load music
	//
	audioManager->PlayMusic("Assets/Audio/Music/map2_music.ogg");

	//
	// Move Camera to starting position
	//
	//app->render->camera.x = -((int)app->win->GetScale() * TILE_SIZE * 2);
	//app->render->camera.y = -((int)app->win->GetScale() * TILE_SIZE * 21);
	// TODO: Fix camera starting position


	return true;
}

// Called each loop iteration
bool Level2::Update(Input* input, float dt)
{
	//
	// Player Update
	//
	player->Update(dt);



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
		player->position = player->checkpointPos;
		//app->player->cameraCollider->SetPos(app->player->checkpointPos.x, app->player->checkpointPos.y - TILE_SIZE * 4);
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

	//
	// Scene change
	//
	if (player->isWinning == true)
	{
		TransitionToScene(SceneType::WIN);
	}

	if (player->isDying == true)
	{
		if (player->lifes <= 0)
		{
			TransitionToScene(SceneType::LOSE);
		}
	}

	return true;
}

// Called each loop iteration
bool Level2::Draw(Render* render)
{
	//
	// Draw Map
	//
	map->Draw(render);

	//
	// Draw Player
	//
	player->Draw(render);


	SDL_Rect lifesRect;
	SDL_Rect cpRect;

	// Lifes HUD Draw
	//lifesRect.x = app->player->cameraCollider->rect.x - (TILE_SIZE * 5);
	//lifesRect.y = app->player->cameraCollider->rect.y - (TILE_SIZE * 4);


	/*for (int i = 0; i < app->player->lifes; i++)
	{
		render->DrawTexture(app->player->lifesTexture, lifesRect.x + 17 * i, lifesRect.y);
	}*/

	// Checkpoint Draw
	//app->render->DrawTexture(app->level2->cpTexture, TILE_SIZE * 44, TILE_SIZE * 20 - 4, &cpRect);


	return false;
}


// Called before quitting
bool Level2::Unload(Textures* tex, AudioManager* audioManager)
{
	if (!active)
	{
		return true;
	}

	LOG("Freeing Level 2");

	map->CleanUp();
	player->CleanUp(tex, audioManager);


	active = false;

	return true;
}