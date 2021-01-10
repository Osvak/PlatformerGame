#ifndef __SCENE_H__
#define __SCENE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"



class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;

class GUIControl;

enum class SceneType
{
	NONE = 0,
	LOGO,
	TITLE,
	LEVEL1,
	LEVEL2,
	WIN,
	LOSE,
	SETTINGS,
	PAUSE
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
	virtual bool OnGUIMouseClickEvent(GUIControl* control)
	{
		return true;
	}


public:

	SceneType nextScene;
	bool active = false;
	SString name;

	bool loaded = false;
	bool transitionRequired;

	bool freeCamera = false;
	bool drawColliders = false;
	bool drawGUI = false;
	bool playerGodMode = false;
	bool exitGame = false;
	bool menuSettings = false;
	bool exitMenuSettings = false;
};

#endif // !__SCENE_H__