#include "MenuPause.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Fonts.h"
#include "AudioManager.h"

#include "Map.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "GUICheckBox.h"

#include "Log.h"


// Constructor
MenuPause::MenuPause(Window* win, Input* input, Render* render, Textures* tex, AudioManager* audioManager, Fonts* fonts)
{
	LOG("Creating Pause Menu");

	name.Create("menuPause");


	this->win = win;
	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->fonts = fonts;
}
// Destructor
MenuPause::~MenuPause()
{
}


// Called before the first frame / when activated
bool MenuPause::Load()
{
	LOG("Loading Pause Menu");
	
	//
	// Load buttons
	//
	GUITexture = tex->Load("Assets/Textures/GUI/gui_spritesheet.png");
	buttonResume = new GUIButton(11, { 16, 65, 100, 24 }, "RESUME", audioManager, fonts);
	buttonResume->SetObserver(this);
	buttonResume->SetTexture(GUITexture);
	buttonSettings = new GUIButton(12, { 16, 95, 100, 24 }, "SETTINGS", audioManager, fonts);
	buttonSettings->SetObserver(this);
	buttonSettings->SetTexture(GUITexture);
	buttonTitle = new GUIButton(13, { 16, 125, 100, 24 }, "TITLE", audioManager, fonts);
	buttonTitle->SetObserver(this);
	buttonTitle->SetTexture(GUITexture);
	buttonExit = new GUIButton(14, { 16, 155, 100, 24 }, "EXIT", audioManager, fonts);
	buttonExit->SetObserver(this);
	buttonExit->SetTexture(GUITexture);

	//
	// Load white font
	//
	font = fonts->Load("Assets/Fonts/font_big_white.png", "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ", 2, 324, 32);

	//
	// Move camera
	//
	render->camera.x = render->camera.y = 0;

	return false;
}


// Called each loop iteration
bool MenuPause::Update(float dt)
{
	//
	// Camera Update
	//
	render->camera.x = 0;
	render->camera.y = 0;

	//
	// Buttons update
	//
	buttonResume->Update(input, dt);
	buttonSettings->Update(input, dt);
	buttonTitle->Update(input, dt);
	buttonExit->Update(input, dt);
	

	//
	// Input update
	//
	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		exitMenuPause = true;
	}


	return false;
}


bool MenuPause::Draw()
{
	//
	// Draw background
	//
	render->DrawRectangle(render->camera, 0, 0, 0, 235);

	//
	// Draw title
	//
	fonts->DrawText(16, 27, font, "Pause");

	//
	// Draw buttons
	//
	buttonResume->Draw(render, drawGUI);
	buttonSettings->Draw(render, drawGUI);
	buttonTitle->Draw(render, drawGUI);
	buttonExit->Draw(render, drawGUI);



	return false;
}


// Called before quitting, frees memory and controls active and inactive modules
bool MenuPause::Unload()
{
	if (!active)
	{
		return false;
	}

	LOG("Unloading Pause Menu");

	tex->UnLoad(GUITexture);

	delete buttonResume;
	buttonResume = nullptr;
	delete buttonSettings;
	buttonSettings = nullptr;
	delete buttonTitle;
	buttonTitle = nullptr;
	delete buttonExit;
	buttonExit = nullptr;

	active = false;

	return false;
}


bool MenuPause::OnGUIMouseClickEvent(GUIControl* control)
{
	switch (control->type)
	{
	case GUIControlType::BUTTON:
	{
		if (control->id == 11) // Resume Button
		{
			exitMenuPause = true;
		}
		if (control->id == 12) // Settings Button
		{
			menuSettings = true;
		}
		if (control->id == 13) // Title Button
		{
			goToTitleScreen = true;
		}
		if (control->id == 14) // Exit Button
		{
			exitGame = true;
		}

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