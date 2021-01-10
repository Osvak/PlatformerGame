#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"

#include "Scene.h"


class GuiButton;

class Input;
class Render;
class Textures;
class Fonts;
class AudioManager;
class EntityManager;
class PathFinding;

class SceneManager : public Module
{
public:

	// Constructor
	SceneManager(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager, PathFinding* pathFinding, Fonts* fonts);
	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);
	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();
	// Called each loop iteration
	bool Update(float dt);
	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

private:

	Scene* current = nullptr;
	Scene* next = nullptr;

	// Required variables to manage screen transitions (fade-in, fade-out)
	bool onTransition;
	bool fadeOutCompleted;
	float transitionAlpha;

	bool freeCamera = false;

public:

	bool fpsCapState = true;

	mutable bool saveGameRequested = false;
	bool loadGameRequested = false;

	Input* input;
	Render* render;
	Textures* tex;
	Fonts* fonts;
	AudioManager* audioManager;
	EntityManager* entityManager;
	PathFinding* pathFinding;
};

#endif // __SCENEMANAGER_H__