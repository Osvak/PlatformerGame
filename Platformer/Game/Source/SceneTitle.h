#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Module.h"

#define BLINK_TIME 35


struct SDL_Texture;

class SceneTitle : public Module
{
public:
	// Constructor
	SceneTitle();

	// Destructor
	~SceneTitle();


	// Called before render is available
	bool Awake();

	// Called before the first frame / when activated
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called after Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	SDL_Texture* pressEnterToStart;

	int blinkCont = 0;

};

#endif // !__SCENETITLE_H__