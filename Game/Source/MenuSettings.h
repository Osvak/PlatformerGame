#ifndef __MENUSETTINGS_H__
#define __MENUSETTINGS_H__

#include "Scene.h"



class Input;
class Render;
class Textures;
class AudioManager;
class Fonts;

class Map;
class GUIButton;
class GUISlider;
class GUICheckBox;

struct SDL_Texture;

class MenuSettings : public Scene
{
public:

	// Constructor
	MenuSettings(Input* input, Render* render, Textures* tex, AudioManager* audioManager, Fonts* fonts);
	// Destructor
	virtual ~MenuSettings();


	// Called before the first frame / when activated
	bool Load();

	// Called each loop iteration
	bool Update(float dt);

	// Called after Updates
	bool Draw();


	// Called before quitting
	bool Unload();


	bool OnGUIMouseClickEvent(GUIControl* control);

private:

	int font = 0;

	SDL_Texture* GUITexture = nullptr;
	GUIButton* buttonBack = nullptr;
	GUISlider* sliderMusic = nullptr;
	GUISlider* sliderFX = nullptr;
	GUICheckBox* checkBoxFullScreen = nullptr;
	GUICheckBox* checkBoxVSync = nullptr;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	Fonts* fonts;
};

#endif // !__MENUSETTINGS_H__