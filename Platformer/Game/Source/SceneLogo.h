#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Module.h"



struct SDL_Texture;

class SceneLogo : public Module
{
public:
	// Constructor
	SceneLogo();

	// Destructor
	~SceneLogo();


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
	SDL_Texture* img;
	int imgW = 0, imgH = 0;
};

#endif // !__SCENELOGO_H__