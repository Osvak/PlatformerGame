#ifndef  __SCENELOSE_H__
#define  __SCENELOSE_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"


class SceneLose : public Scene
{
public:

	// Constructor
	SceneLose();
	// Destructor
	virtual ~SceneLose();


	// Called before the first frame / when activated
	bool Load(Textures* tex);

	// Called each loop iteration
	bool Update(Input* input, float dt);

	// Called after Updates
	bool Draw(Render* render);


	// Called before quitting
	bool Unload();

private:
	unsigned int gameOverFX;
	bool playFX = true;
};

#endif // ! __SCENELOSE_H__
