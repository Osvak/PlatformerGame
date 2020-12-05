#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Module.h"

#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Input.h"
#include "Map.h"
#include "FadeToBlack.h"



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
};

#endif // !__SCENETITLE_H__