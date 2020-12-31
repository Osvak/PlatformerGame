#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

#include "Map.h"


//#include "GuiButton.h"

#define BLINK_TIME 35


class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;

struct SDL_Texture;

class SceneTitle : public Scene
{
public:

	// Constructor
	SceneTitle(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager);
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

private:

	SDL_Texture* pressEnterToStart = nullptr;
	int blinkCont = 0;

	Map* map = nullptr;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	EntityManager* entityManager;
};

#endif // !__SCENETITLE_H__