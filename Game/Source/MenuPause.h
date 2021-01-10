#ifndef __MENUPAUSE_H__
#define __MENUPAUSE_H__

#include "Scene.h"



class Window;
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

class MenuPause : public Scene
{
public:

	// Constructor
	MenuPause(Window* win, Input* input, Render* render, Textures* tex, AudioManager* audioManager, Fonts* fonts);
	// Destructor
	virtual ~MenuPause();


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
	GUIButton* buttonResume = nullptr;
	GUIButton* buttonSettings = nullptr;
	GUIButton* buttonTitle = nullptr;
	GUIButton* buttonExit = nullptr;

	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	Fonts* fonts;
};

#endif // !__MENUPAUSE_H__