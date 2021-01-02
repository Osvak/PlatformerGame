#ifndef __LEVEL2_H__
#define __LEVEL2_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"



class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;

class Level2 : public Scene
{
public:

	// Constructor
	Level2(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager);
	// Destructor
	virtual ~Level2();


	// Called before the first frame
	bool Load();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool Draw();


	// Called before quitting
	bool Unload();

private:

	Map* map = nullptr;
	Player* player = nullptr;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	EntityManager* entityManager;
};

#endif // __LEVEL2_H__