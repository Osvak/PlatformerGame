#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include "Module.h"

#include "Animation.h"

struct SDL_Texture;



class Level1 : public Module
{
public:

	Level1();

	// Destructor
	virtual ~Level1();


	// Called before render is available
	bool Awake(pugi::xml_node& config);

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

public:
	bool freeCamera = false;
};

#endif // __LEVEL1_H__