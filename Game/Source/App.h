#ifndef __APP_H__
#define __APP_H__

#include "Module.h"

#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "PugiXml/src/pugixml.hpp"



// Modules
class Window;
class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;
class SceneManager;

// Main Application
class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();


	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();


	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

    // Create methods to request Load / Save
	void LoadConfigRequested();
	void SaveConfigRequested() const;

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;


private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGame(const SString _fileName);
	bool SaveGame(const SString _fileName) const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	EntityManager* entityManager;
	SceneManager* sceneManager;

	// Public FPS variables
	int highFrameRate = 60;
	int lowFrameRate = 30;
	int highScreenTicks;
	int lowScreenTicks;

	mutable bool saveConfigRequested = false;
	bool loadConfigRequested = false;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;


	SString filenameGame = "save_game.xml";
	SString filenameConfig = "config.xml";

	pugi::xml_document saveStateFile;
	pugi::xml_node nodeSaveStateFile;



	// FPS variables
	PerfTimer pTimer;
	uint64 frameCount = 0;
	Timer startupTime;
	Timer dtTimer;
	float dt = 0.0f;
	float secondsSinceStartup;
	float fpsMSeconds;
	float fpsCounter;
	float fps = 0.f;
	float fpsPreUpdate;
};

#endif	// __APP_H__