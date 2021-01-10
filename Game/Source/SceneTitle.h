#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

#define BLINK_TIME 35



class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;
class Fonts;

class Map;
class GUIButton;

struct SDL_Texture;

class SceneTitle : public Scene
{
public:

	// Constructor
	SceneTitle(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager, Fonts* fonts);
	// Destructor
	virtual ~SceneTitle();


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

	Map* map = nullptr;

	SDL_Texture* GUITexture = nullptr;
	GUIButton* buttonPlay = nullptr;
	GUIButton* buttonContinue = nullptr;
	GUIButton* buttonSettings = nullptr;
	GUIButton* buttonCredits = nullptr;
	GUIButton* buttonExit = nullptr;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	EntityManager* entityManager;
	Fonts* fonts;
};

#endif // !__SCENETITLE_H__