#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"

#include "Scene.h"


class GuiButton;

class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;

class SceneManager : public Module
{
public:

	// Constructor
	SceneManager(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager);
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

private:

	Scene* current;
	Scene* next;

	// Required variables to manage screen transitions (fade-in, fade-out)
	bool onTransition;
	bool fadeOutCompleted;
	float transitionAlpha;

	bool freeCamera = false;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	EntityManager* entityManager;
};

#endif // __SCENEMANAGER_H__