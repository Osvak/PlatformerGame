#ifndef  __SCENELOSE_H__
#define  __SCENELOSE_H__

#include "Module.h"

class SceneLose : public Module
{
public:
	// Constructor
	SceneLose();

	// Destructor
	~SceneLose();


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
	unsigned int gameOverFX;
	bool playFX = true;
};

#endif // ! __SCENELOSE_H__
