#ifndef __LEVEL2_H__
#define __LEVEL2_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"
#include "Animation.h"

struct SDL_Texture;


class Level2 : public Scene
{
public:

	// Constructor
	Level2();
	// Destructor
	virtual ~Level2();


	// Called before the first frame
	bool Load(Textures* tex);

	// Called each loop iteration
	bool Update(Input* input, float dt);

	// Called before all Updates
	bool Draw(Render* render);


	// Called before quitting
	bool Unload();


	// Checkpoint activation
	bool Cp2Activation();


public:
	Collider* checkPointCollider = nullptr;
	SDL_Texture* cpTexture = nullptr;

	Animation* currentAnim;

	Animation* cpIdleAnim = new Animation();
	Animation* cpActiveAnim = new Animation();



	// Checkpoint flags
	bool isCpActive = false;
};

#endif // __LEVEL2_H__