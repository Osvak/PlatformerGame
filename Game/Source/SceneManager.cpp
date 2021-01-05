#include "SceneManager.h"

#include "SceneLogo.h"
#include "SceneTitle.h"
#include "Level1.h"
#include "Level2.h"
#include "SceneWin.h"
#include "SceneLose.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Pathfinding.h"
//#include "GuiButton.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f


// Constructor
SceneManager::SceneManager(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager, PathFinding* pathFinding) : Module()
{
	name.Create("sceneManager");

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;;

	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->entityManager = entityManager;
	this->pathFinding = pathFinding;
}

// Destructor
SceneManager::~SceneManager()
{}


// Called before render is available
bool SceneManager::Awake(pugi::xml_node&)
{
	LOG("SceneManager.Awake(): Loading Scene manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	LOG("SceneManager.Start(): Making SceneLogo be the first Scene");

	current = new SceneLogo(input, render, tex, audioManager);
	current->Load();
	current->name = "sceneLogo";

	next = nullptr;

	return true;
}


// Called each loop iteration
bool SceneManager::PreUpdate()
{
	/*
	// L12b: Debug pathfing
	static iPoint origin;
	static bool originSelected = false;

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);

	if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(originSelected == true)
		{
			app->pathFinding->CreatePath(origin, p);
			originSelected = false;
		}
		else
		{
			origin = p;
			originSelected = true;
		}
	}
	*/

	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	if (!onTransition)
	{
		if (input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			saveGameRequested = true;
		}
		if (input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		{
			loadGameRequested = true;
		}
		if (input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		{
			freeCamera = !freeCamera;
		}
		if (input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		{
			fpsCapState = !fpsCapState;
		}

		if (freeCamera == true)
		{
			if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) render->camera.y += 5;
			if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) render->camera.y -= 5;
			if (input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) render->camera.x += 5;
			if (input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) render->camera.x -= 5;
		}

		current->Update(dt);
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f)
			{
				transitionAlpha = 1.0f;

				LOG("CHANGING SCENE FROM %s TO %s", current->name.GetString(), next->name.GetString());

				current->Unload();	// Unload current screen
				next->Load();	// Load next screen

				RELEASE(current);	// Free current pointer
				current = next;		// Assign next pointer
				next = nullptr;

				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;
			}
		}
		else  // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	// Draw current scene
	current->Draw();

	// Draw full screen rectangle in front of everything
	if (onTransition)
	{
		render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, (unsigned char)(255.0f * transitionAlpha));
	}

	// L12b: Debug pathfinding
	/*
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);
	p = app->map->MapToWorld(p.x, p.y);

	const DynArray<iPoint>* path = app->pathFinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(debugTex, pos.x, pos.y);
	}
	*/

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
		case SceneType::LOGO: next = new SceneLogo(input, render, tex, audioManager); break;
		case SceneType::TITLE: next = new SceneTitle(input, render, tex, audioManager, entityManager); break;
		case SceneType::LEVEL1: next = new Level1(input, render, tex, audioManager, entityManager, pathFinding); break;
		case SceneType::LEVEL2: next = new Level2(input, render, tex, audioManager, entityManager); break;
		case SceneType::WIN: next = new SceneWin(input, render, tex, audioManager, entityManager); break;
		case SceneType::LOSE: next = new SceneLose(input, render, tex, audioManager, entityManager); break;
		default: break;
		}

		current->transitionRequired = false;
	}

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;
	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	bool ret = true;

	return ret;
}


// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing Current Scene");

	if (current != nullptr) current->Unload();

	return true;
}


// Save/Load scene info
bool SceneManager::LoadState(pugi::xml_node& sceneNode)
{
	// TODO: Load SceneManager
	const char* n = sceneNode.child("scene").attribute("current").as_string();
	SString savedScene(n);
	if (savedScene == "level1")
	{
		current->nextScene = SceneType::LEVEL1;
		current->transitionRequired = true;
	}
	if (savedScene == "level2")
	{
		current->nextScene = SceneType::LEVEL2;
		current->transitionRequired = true;
	}
	
	return true;
}

bool SceneManager::SaveState(pugi::xml_node& sceneNode) const
{
	// TODO: Save SceneManager
	pugi::xml_node scene = sceneNode.append_child("scene");
	scene.append_attribute("current").set_value(current->name.GetString());

	return true;
}