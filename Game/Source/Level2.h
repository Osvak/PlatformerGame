#ifndef __LEVEL2_H__
#define __LEVEL2_H__

#include "Module.h"

#include "Animation.h"

struct SDL_Texture;

class Level2 : public Module
{
public:

	Level2();

	// Destructor
	virtual ~Level2();


	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

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

public:
	bool freeCamera = false;


};

#endif // __LEVEL2_H__