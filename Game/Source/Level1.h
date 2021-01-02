#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"



class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;

class Level1 : public Scene
{
public:

	// Constructor
	Level1();
	// Destructor
	virtual ~Level1();


	// Called before the first frame
	bool Load(Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager);

	// Called each loop iteration
	bool Update(Input* input, AudioManager* audioManager, float dt);

	// Called before all Updates
	bool Draw(Render* render);


	// Called before quitting
	bool Unload(Textures* tex, AudioManager* audioManager, EntityManager* entityManager);

private:

	Map* map = nullptr;
	Player* player = nullptr;
};

#endif // __LEVEL1_H__