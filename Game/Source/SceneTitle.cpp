#include "SceneTitle.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "EntityManager.h"

#include "Map.h"
#include "GUIButton.h"

#include "Log.h"


// Constructor
SceneTitle::SceneTitle(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
{
	LOG("Creating Title Screen");

	name.Create("sceneTitle");


	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->entityManager = entityManager;
}

// Destructor
SceneTitle::~SceneTitle()
{
}



// Called before the first frame / when activated
bool SceneTitle::Load()
{
	LOG("Loading Title Screen");

	//
	// Load map
	//
	map = (Map*)entityManager->CreateEntity(EntityType::MAP);
	map->Load("scene_title.tmx");

	//
	// Load buttons
	//
	GUITexture = tex->Load("Assets/Textures/GUI/gui_spritesheet.png");
	buttonPlay = new GUIButton(1, { 56, 128, 100, 24 }, "PLAY", audioManager);
	buttonPlay->SetObserver(this);
	buttonPlay->SetTexture(GUITexture);
	buttonContinue = new GUIButton(2, { 166, 128, 100, 24 }, "CONTINUE", audioManager);
	buttonContinue->SetObserver(this);
	buttonContinue->SetTexture(GUITexture);
	buttonSettings = new GUIButton(3, { 276, 128, 100, 24 }, "SETTINGS", audioManager);
	buttonSettings->SetObserver(this);
	buttonSettings->SetTexture(GUITexture);
	buttonCredits = new GUIButton(4, { 111, 176, 100, 24 }, "CREDITS", audioManager);
	buttonCredits->SetObserver(this);
	buttonCredits->SetTexture(GUITexture);
	buttonExit = new GUIButton(5, { 221, 176, 100, 24 }, "EXIT", audioManager);
	buttonExit->SetObserver(this);
	buttonExit->SetTexture(GUITexture);

	//
	// Load music
	//
	audioManager->PlayMusic("Assets/Audio/Music/title_screen_music.ogg");

	//
	// Move camera
	//
	render->camera.x = render->camera.y = 0;

	return false;
}



// Called each loop iteration
bool SceneTitle::Update(float dt)
{
	//
	// Scene controls
	//
	if (input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
		input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN)
	{
		TransitionToScene(SceneType::LEVEL1);
		return false;
	}

	//
	// Buttons update
	//
	buttonPlay->Update(input, dt);
	buttonContinue->Update(input, dt);
	buttonSettings->Update(input, dt);
	buttonCredits->Update(input, dt);
	buttonExit->Update(input, dt);


	return false;
}


bool SceneTitle::Draw()
{
	//
	// Draw map
	//
	map->Draw();

	//
	// Draw buttons
	//
	buttonPlay->Draw(render, drawGUI);
	buttonContinue->Draw(render, drawGUI);
	buttonSettings->Draw(render, drawGUI);
	buttonCredits->Draw(render, drawGUI);
	buttonExit->Draw(render, drawGUI);

	return false;
}


// Called before quitting, frees memory and controls active and inactive modules
bool SceneTitle::Unload()
{
	if (!active)
	{
		return false;
	}

	LOG("Unloading Title Sreen");

	tex->UnLoad(GUITexture);
	entityManager->DestroyEntity(map);

	delete buttonPlay;
	buttonPlay = nullptr;
	delete buttonContinue;
	buttonContinue = nullptr;
	delete buttonSettings;
	buttonSettings = nullptr;
	delete buttonCredits;
	buttonCredits = nullptr;
	delete buttonExit;
	buttonExit = nullptr;


	active = false;

	return false;
}


bool SceneTitle::OnGUIMouseClickEvent(GUIControl* control)
{
	switch (control->type)
	{
	case GUIControlType::BUTTON:
	{
		break;
	}
	
	case GUIControlType::CHECKBOX:
	{
		break;
	}

	case GUIControlType::SLIDER:
	{
		break;
	}

	default:
		break;
	}


	return true;
}