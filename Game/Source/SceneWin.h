#ifndef __SCENEWIN_H__
#define __SCENEWIN_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"


class SceneWin : public Scene
{
public:

	// Constructor
	SceneWin();
	// Destructor
	virtual ~SceneWin();


	// Called before the first frame / when activated
	bool Load(Textures* tex);

	// Called each loop iteration
	bool Update(Input* input, float dt);

	// Called after Updates
	bool Draw(Render* render);


	// Called before quitting
	bool Unload();

private:
	unsigned int victoryFX;
	bool playFX = true;
};

#endif // !__SCENETITLE_H__