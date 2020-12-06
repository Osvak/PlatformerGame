#include "SceneLogo.h"

#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"
#include "Map.h"
#include "FadeToBlack.h"

#include "Log.h"


// Constructor
SceneLogo::SceneLogo() : Module()
{
	name.Create("sceneLogo");
}

// Destructor
SceneLogo::~SceneLogo()
{

}

// Called before render is available
bool SceneLogo::Awake()
{
	LOG("Loading SceneLogo");
	bool ret = true;
	return ret;
}

// Called before the first frame
bool SceneLogo::Start()
{
	app->currentScene = LOGO;

	img = app->tex->Load("Assets/maps/scene_logo.png");

	logoFX = app->audio->LoadFX("Assets/audio/fx/logo.wav");
	app->musicList.Add(&logoFX);

	imgW = (int)app->win->GetWidth() / (int)app->win->GetScale();
	imgH = (int)app->win->GetHeight() / (int)app->win->GetScale();

	if (SDL_QueryTexture(img, NULL, NULL, &imgW, &imgH) != 0)
	{
		LOG("Error on QueryTexture");
	}

	app->render->camera.x = app->render->camera.y = 0;

	nextSceneCounter = 0;
	transition = false;


	return true;
}

// Called before all Updates
bool SceneLogo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLogo::Update(float dt)
{
	if (nextSceneCounter < NEXT_SCENE_TIME && transition == false)
	{
		++nextSceneCounter;
		if (nextSceneCounter == 15)
		{
			app->audio->PlayFX(logoFX);
		}
	}

	return true;
}

//Called after Updates
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	//
	// Drawp map
	//
	if (app->render->DrawTexture(img, 0, 0) == false)
	{
		LOG("Error drawing Scene Logo");
	}


	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN
		|| app->input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN || nextSceneCounter == NEXT_SCENE_TIME)
	{
		app->fadeToBlack->Fade(this, (Module*)app->sceneTitle, 60.0f);
		transition = true;
		nextSceneCounter = 0;
		return true;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, (Module*)app->scene, 60.0f);
		transition = true;
		nextSceneCounter = 0;
		return true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, (Module*)app->scene2, 60.0f);
		transition = true;
		nextSceneCounter = 0;
		return true;
	}

	return ret;
}

// Called before quitting, frees memory
bool SceneLogo::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Freeing Logo Screen");

	app->tex->UnLoad(img);

	app->audio->UnloadFX(logoFX);

	app->map->CleanUp();
	active = false;

	return true;
}