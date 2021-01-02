#ifndef __SCENEWIN_H__
#define __SCENEWIN_H__

#include "Scene.h"

#include "Map.h"



class SceneWin : public Scene
{
public:

	// Constructor
	SceneWin();
	// Destructor
	virtual ~SceneWin();


	// Called before the first frame / when activated
	bool Load(Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager);

	// Called each loop iteration
	bool Update(Input* input, AudioManager* audioManager, float dt);

	// Called after Updates
	bool Draw(Render* render);


	// Called before quitting
	bool Unload(Textures* tex, AudioManager* audioManager, EntityManager* entityManager);

private:

	unsigned int victoryFX;
	bool playFX = true;

	Map* map = nullptr;
};

#endif // !__SCENETITLE_H__