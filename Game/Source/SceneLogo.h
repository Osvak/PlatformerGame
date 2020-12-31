#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"

#define NEXT_SCENE_TIME 80


struct SDL_Texture;

class SceneLogo : public Scene
{
public:

	// Constructor
	SceneLogo();
	// Destructor
	virtual ~SceneLogo();


	// Called before the first frame / when activated
	bool Load(Textures* tex, EntityManager* entityManager, AudioManager* audioManager);


	// Called each loop iteration
	bool Update(Input* input, float dt);

	bool Draw(Render* render);


	// Called before quitting
	bool Unload(Textures* tex, AudioManager* audioManager);


private:
	SDL_Texture* img = nullptr;
	unsigned int logoFX;
	int imgW = 0, imgH = 0;
	bool transition = false;
	int nextSceneCounter = 0;
};

#endif // !__SCENELOGO_H__