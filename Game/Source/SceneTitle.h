#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

#include "Map.h"


//#include "GuiButton.h"

#define BLINK_TIME 35


struct SDL_Texture;

class SceneTitle : public Scene
{
public:

	// Constructor
	SceneTitle();
	// Destructor
	virtual ~SceneTitle();


	// Called before the first frame / when activated
	bool Load(Textures* tex, EntityManager* entityManager, AudioManager* audioManager);

	// Called each loop iteration
	bool Update(Input* input, float dt);

	// Called after Updates
	bool Draw(Render* render);


	// Called before quitting
	bool Unload(Textures* tex, AudioManager* audioManager);

private:

	Map* map = nullptr;
	SDL_Texture* pressEnterToStart = nullptr;

	int blinkCont = 0;
};

#endif // !__SCENETITLE_H__