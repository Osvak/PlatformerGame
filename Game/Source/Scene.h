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
	
	virtual bool Load(Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
	{
		return true;
	}

	virtual bool Update(Input* input, AudioManager* audioManager, float dt)
	{
		return true;
	}

	virtual bool Draw(Render* render)
	{
		return true;
	}

	virtual bool Unload(Textures* tex, AudioManager* audioManager, EntityManager* entityManager)
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