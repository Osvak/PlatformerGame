#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

#include "Animation.h"

struct SDL_Texture;



class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();


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
	bool Cp1Activation();


public:
	Collider* checkPointCollider = nullptr;
	SDL_Texture* cpTexture = nullptr;

	Animation* currentAnim;

	Animation* cpIdleAnim = new Animation();
	Animation* cpActiveAnim = new Animation();
	


	// Checkpoint flags
	bool isCpActive = false;
};

#endif // __SCENE_H__