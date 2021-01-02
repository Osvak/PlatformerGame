#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"

#include "Map.h"


#define NEXT_SCENE_TIME 6000



class SDL_Texture;

class SceneLogo : public Scene
{
public:

	// Constructor
	SceneLogo();
	// Destructor
	virtual ~SceneLogo();


	// Called before the first frame / when activated
	bool Load(Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager);


	// Called each loop iteration
	bool Update(Input* input, AudioManager* audioManager, float dt);

	bool Draw(Render* render);


	// Called before quitting
	bool Unload(Textures* tex, AudioManager* audioManager, EntityManager* entityManager);


private:

	SString name;

	unsigned int logoFX;
	int imgW = 0, imgH = 0;
	bool transition = false;
	int nextSceneCounter = 0;

	//Map* map;
	SDL_Texture* img = nullptr;
};

#endif // !__SCENELOGO_H__