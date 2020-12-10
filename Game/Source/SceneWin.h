#ifndef __SCENEWIN_H__
#define __SCENEWIN_H__

#include "Module.h"


class SceneWin : public Module
{
public:
	// Constructor
	SceneWin();

	// Destructor
	~SceneWin();


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
	unsigned int victoryFX;
	bool playFX = true;
};

#endif // !__SCENETITLE_H__