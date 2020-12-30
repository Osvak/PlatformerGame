#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"
#include "Animation.h"

struct SDL_Texture;


class Level1 : public Scene
{
public:

	// Constructor
	Level1();
	// Destructor
	virtual ~Level1();


	// Called before the first frame
	bool Load(Textures* tex);

	// Called each loop iteration
	bool Update(Input* input, float dt);

	// Called before all Updates
	bool Draw(Render* render);


	// Called before quitting
	bool Unload();


	// Checkpoint activation
	bool Cp1Activation();


public:
	// Checkpoint Variables
	Collider* checkPointCollider = nullptr;
	SDL_Texture* cpTexture = nullptr;

	Animation* currentAnim;

	Animation* cpIdleAnim = new Animation();
	Animation* cpActiveAnim = new Animation();

	// Checkpoint flags
	bool isCpActive = false;
};

#endif // __LEVEL1_H__