#ifndef __LEVEL2_H__
#define __LEVEL2_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"


struct SDL_Texture;


class Level2 : public Scene
{
public:

	// Constructor
	Level2();
	// Destructor
	virtual ~Level2();


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

#endif // __LEVEL2_H__