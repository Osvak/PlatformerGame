#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

#include "Map.h"
//#include "GuiButton.h"


#define BLINK_TIME 35



class SceneTitle : public Scene
{
public:

	// Constructor
	SceneTitle();
	// Destructor
	virtual ~SceneTitle();


	// Called before the first frame / when activated
	bool Load(Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager);

	// Called each loop iteration
	bool Update(Input* input, AudioManager* audioManager, float dt);

	// Called after Updates
	bool Draw(Render* render);


	// Called before quitting
	bool Unload(Textures* tex, AudioManager* audioManager, EntityManager* entityManager);

private:

	SDL_Texture* pressEnterToStart = nullptr;
	int blinkCont = 0;

	Map* map = nullptr;
};

#endif // !__SCENETITLE_H__