#ifndef  __SCENELOSE_H__
#define  __SCENELOSE_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"


class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;

class SceneLose : public Scene
{
public:

	// Constructor
	SceneLose(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager);
	// Destructor
	virtual ~SceneLose();


	// Called before the first frame / when activated
	bool Load();

	// Called each loop iteration
	bool Update(float dt);

	// Called after Updates
	bool Draw();


	// Called before quitting
	bool Unload();

private:

	unsigned int gameOverFX;
	bool playFX = true;

	Map* map;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	EntityManager* entityManager;
};

#endif // ! __SCENELOSE_H__
