#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"

#include "Map.h"


#define NEXT_SCENE_TIME 200


class Input;
class Render;
class Textures;
class AudioManager;

struct SDL_Texture;

class SceneLogo : public Scene
{
public:

	// Constructor
	SceneLogo(Input* input, Render* render, Textures* tex, AudioManager* audioManager);
	// Destructor
	virtual ~SceneLogo();


	// Called before the first frame / when activated
	bool Load();


	// Called each loop iteration
	bool Update(float dt);

	bool Draw();


	// Called before quitting
	bool Unload();


private:

	SString name;

	unsigned int logoFX;
	int imgW = 0, imgH = 0;
	bool transition = false;
	int nextSceneCounter = 0;

	//Map* map;
	SDL_Texture* img = nullptr;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
};

#endif // !__SCENELOGO_H__