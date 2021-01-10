#include "MenuSettings.h"

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
MenuSettings::MenuSettings(Window* win, Input* input, Render* render, Textures* tex, AudioManager* audioManager, Fonts* fonts)
{
	LOG("Creating Title Screen");

	name.Create("sceneTitle");


	this->win = win;
	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->fonts = fonts;
}
// Destructor
MenuSettings::~MenuSettings()
{
}


// Called before the first frame / when activated
bool MenuSettings::Load()
{
	LOG("Loading Title Screen");

	//
	// Load buttons
	//
	GUITexture = tex->Load("Assets/Textures/GUI/gui_spritesheet.png");
	buttonBack = new GUIButton(6, { 163, 205, 100, 24 }, "BACK", audioManager, fonts);
	buttonBack->SetObserver(this);
	buttonBack->SetTexture(GUITexture);
	sliderMusic = new GUISlider(7, { 163, 65, 100, 10 }, audioManager, fonts, "Music Volume", audioManager->GetMusicVolume(), 0, 100);
	sliderMusic->SetObserver(this);
	sliderMusic->SetTexture(GUITexture);
	sliderFX = new GUISlider(8, { 163, 95, 100, 10 }, audioManager, fonts, "FX Volume", audioManager->GetFXVolume(), 0, 100);
	sliderFX->SetObserver(this);
	sliderFX->SetTexture(GUITexture);
	checkBoxFullScreen = new GUICheckBox(9, { 206, 125, 15, 15 }, "Full Screen", audioManager, fonts, false);
	checkBoxFullScreen->SetObserver(this);
	checkBoxFullScreen->SetTexture(GUITexture);
	checkBoxVSync = new GUICheckBox(10, { 206, 155, 15, 15 }, "VSync", audioManager, fonts, false);
	checkBoxVSync->SetObserver(this);
	checkBoxVSync->SetTexture(GUITexture);

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
bool MenuSettings::Update(float dt)
{
	//
	// Buttons update
	//
	buttonBack->Update(input, dt);

	//
	// Sliders update
	//
	sliderMusic->Update(input, dt);
	sliderFX->Update(input, dt);

	//
	// CheckBoxes update
	//
	checkBoxFullScreen->Update(input, dt);
	checkBoxVSync->Update(input, dt);

	return false;
}


bool MenuSettings::Draw()
{
	//
	// Draw background
	//
	render->DrawRectangle(render->camera, 0, 0, 0, 235);

	//
	// Draw title
	//
	fonts->DrawText(136, 27, font, "Settings Menu");

	//
	// Draw buttons
	//
	buttonBack->Draw(render, drawGUI);

	//
	// Draw sliders
	//
	sliderMusic->Draw(render, drawGUI);
	sliderFX->Draw(render, drawGUI);

	//
	// Draw checkboxes
	//
	checkBoxFullScreen->Draw(render, drawGUI);
	checkBoxVSync->Draw(render, drawGUI);


	return false;
}


// Called before quitting, frees memory and controls active and inactive modules
bool MenuSettings::Unload()
{
	if (!active)
	{
		return false;
	}

	LOG("Unloading Title Sreen");

	tex->UnLoad(GUITexture);

	delete buttonBack;
	buttonBack = nullptr;
	delete sliderMusic;
	sliderMusic = nullptr;
	delete sliderFX;
	sliderFX = nullptr;
	delete checkBoxFullScreen;
	checkBoxFullScreen = nullptr;
	delete checkBoxVSync;
	checkBoxVSync = nullptr;

	active = false;

	return false;
}


bool MenuSettings::OnGUIMouseClickEvent(GUIControl* control)
{
	switch (control->type)
	{
	case GUIControlType::BUTTON:
	{
		if (control->id == 6) // Back Button
		{
			exitMenuSettings = true;
		}


		break;
	}

	case GUIControlType::CHECKBOX:
	{
		if (control->id == 9) // Full Screen checkbox
		{
			if (control->value == 1)
			{
				win->FullScreen(true);
			}
			else
			{
				win->FullScreen(false);
			}
			
		}
		if (control->id == 10) // VSync checkbox
		{
			if (control->value == 1)
			{
				render->ToggleVSync(true);
			}
			else
			{
				render->ToggleVSync(false);
			}
		}


		break;
	}

	case GUIControlType::SLIDER:
	{
		if (control->id == 7) // Music slider
		{
			audioManager->SetMusicVolume(control->value);
		}
		if (control->id == 8) // FX slider
		{
			audioManager->SetFXVolume(control->value);
		}

			

		break;
	}

	default:
		break;
	}


	return true;
}