#include "SceneManager.h"

#include "SceneLogo.h"
#include "SceneTitle.h"
#include "Level1.h"
#include "Level2.h"
#include "SceneWin.h"
#include "SceneLose.h"

#include "MenuSettings.h"
#include "MenuPause.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Pathfinding.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"
#include "Point.h"

#include "SDL/include/SDL.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f


// Constructor
SceneManager::SceneManager(Window* win, Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager, PathFinding* pathFinding, Fonts* fonts) : Module()
{
	name.Create("sceneManager");

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;;

	this->win = win;
	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->entityManager = entityManager;
	this->pathFinding = pathFinding;
	this->fonts = fonts;
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

	menuSettings = new MenuSettings(win, input, render, tex, audioManager, fonts);
	menuSettings->Load();
	menuPause = new MenuPause(win, input, render, tex, audioManager, fonts);
	menuPause->Load();
	current = new SceneLogo(input, render, tex, audioManager);
	current->Load();
	current->name = "sceneLogo";

	next = nullptr;

	return true;
}


// Called each loop iteration
bool SceneManager::PreUpdate()
{


	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	if (!onTransition)
	{
		if (current->menuPause == true && current->menuSettings == false)
		{
			menuPause->exitMenuPause = false;
			menuPause->Update(dt);
			if (menuPause->menuSettings == true)
			{
				current->menuSettings = true;
			}
		}
		if (current->menuSettings == true)
		{
			if (current->menuPause == true)
			{
				menuSettings->drawBackground = false;
			}
			else
			{
				menuSettings->drawBackground = true;
			}
			menuSettings->exitMenuSettings = false;
			menuSettings->Update(dt);
		}
		if (current->exitGame == true || menuPause->exitGame == true)
		{
			return false;
		}
		if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN &&
			current->name != "sceneTitle" &&
			current->name != "level1" &&
			current->name != "level2")
		{
			return false;
		}
		if (menuPause->goToTitleScreen == true)
		{
			menuPause->goToTitleScreen = false;
			current->menuPause = false;
			current->TransitionToScene(SceneType::TITLE);
		}
		if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			current->TransitionToScene(SceneType::LEVEL1);
			return true;
		}
		if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			current->TransitionToScene(SceneType::LEVEL2);
			return true;
		}
		if (input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			current->TransitionToScene(current->nextScene);
			return true;
		}
		if (current->name == "level1" || current->name == "level2")
		{
			if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			{
				if (current->menuPause == false)
				{
					current->savedCameraPositon.x = render->camera.x;
					current->savedCameraPositon.y = render->camera.y;
				}
				current->menuPause = !current->menuPause;
			}
			if (input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
			{
				saveGameRequested = true;
			}
			if (input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			{
				loadGameRequested = true;
			}
			if (input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			{
				current->drawColliders = !current->drawColliders;
			}
			if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			{
				current->playerGodMode = !current->playerGodMode;
			}
		}
		if (input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		{
			current->drawGUI = !current->drawGUI;
			menuSettings->drawGUI = !menuSettings->drawGUI;
		}
		if (input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		{
			fpsCapState = !fpsCapState;
		}

		if (input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			freeCamera = !freeCamera;
			current->freeCamera = !current->freeCamera;
		}
		if (freeCamera == true)
		{
			if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) render->camera.y += 5;
			if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) render->camera.y -= 5;
			if (input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) render->camera.x += 5;
			if (input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) render->camera.x -= 5;
		}

		if (menuSettings->exitMenuSettings == true)
		{
			menuPause->menuSettings = false;
			current->menuSettings = false;
			menuSettings->exitMenuSettings = false;
			render->camera.x = current->savedCameraPositon.x;
		}
		if (menuPause->exitMenuPause == true)
		{
			current->menuPause = false;
			menuPause->exitMenuPause = false;
			render->camera.y = current->savedCameraPositon.y;
		}
		if (current->menuSettings == false && current->menuPause == false)
		{
			current->Update(dt);
		}
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
	// Draw menus on top
	if (current->menuPause == true)
	{
		menuPause->Draw();
	}
	if (current->menuSettings == true)
	{
		menuSettings->Draw();
	}
	

	// Draw full screen rectangle in front of everything
	if (onTransition)
	{
		render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, (unsigned char)(255.0f * transitionAlpha));
	}

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
		case SceneType::LOGO: next = new SceneLogo(input, render, tex, audioManager); break;
		case SceneType::TITLE: next = new SceneTitle(input, render, tex, audioManager, entityManager, fonts); break;
		case SceneType::LEVEL1: next = new Level1(input, render, tex, audioManager, entityManager, pathFinding); break;
		case SceneType::LEVEL2: next = new Level2(input, render, tex, audioManager, entityManager, pathFinding); break;
		case SceneType::WIN: next = new SceneWin(input, render, tex, audioManager, entityManager); break;
		case SceneType::LOSE: next = new SceneLose(input, render, tex, audioManager, entityManager); break;
		default: break;
		}

		current->transitionRequired = false;
	}

	//if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;
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
	pugi::xml_node scene = sceneNode.append_child("scene");
	scene.append_attribute("current").set_value(current->name.GetString());

	return true;
}