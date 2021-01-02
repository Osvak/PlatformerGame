#ifndef __SCENE_H__
#define __SCENE_H__

#include "SString.h"


class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;

//class GuiControl;


enum class SceneType
{
	NONE = 0,
	LOGO,
	TITLE,
	LEVEL1,
	LEVEL2,
	WIN,
	LOSE,
};


class Scene
{
public:

	Scene() : active(true), loaded(false), transitionRequired(false) {}
	
	virtual bool Load()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw()
	{
		return true;
	}

	virtual bool Unload()
	{
		return true;
	}

	void TransitionToScene(SceneType scene)
	{
		transitionRequired = true;
		nextScene = scene;
	}


	// Define multiple Gui Event methods
	/*virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}*/


public:

	SceneType nextScene;
	bool active = false;
	SString name;

	bool loaded = false;
	bool transitionRequired;
};

#endif // !__SCENE_H__