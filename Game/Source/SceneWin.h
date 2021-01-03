#ifndef __SCENEWIN_H__
#define __SCENEWIN_H__

#include "Scene.h"

#include "Map.h"



class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;

class SceneWin : public Scene
{
public:

	// Constructor
	SceneWin(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager);
	// Destructor
	virtual ~SceneWin();


	// Called before the first frame / when activated
	bool Load();

	// Called each loop iteration
	bool Update(float dt);

	// Called after Updates
	bool Draw();


	// Called before quitting
	bool Unload();

private:

	unsigned int victoryFX = 0;
	bool playFX = true;

	Map* map = nullptr;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	EntityManager* entityManager;
};

#endif // !__SCENETITLE_H__