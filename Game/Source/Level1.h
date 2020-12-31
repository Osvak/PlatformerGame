#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"


struct SDL_Texture;


class Level1 : public Scene
{
public:

	// Constructor
	Level1();
	// Destructor
	virtual ~Level1();


	// Called before the first frame
	bool Load(Textures* tex, EntityManager* entityManager, AudioManager* audioManager);

	// Called each loop iteration
	bool Update(Input* input, float dt);

	// Called before all Updates
	bool Draw(Render* render);


	// Called before quitting
	bool Unload(Textures* tex, AudioManager* audioManager);

private:

	Map* map = nullptr;
	Player* player = nullptr;
};

#endif // __LEVEL1_H__