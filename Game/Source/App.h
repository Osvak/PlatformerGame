#ifndef __APP_H__
#define __APP_H__

#include "Module.h"

#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"



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
	void LoadGameRequest();
	void SaveGameRequest() const;
	void LoadConfigRequested();
	void SaveConfigRequested() const;


private:

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

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
	bool LoadGame(const SString _filename);
	bool SaveGame(const SString _filename) const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	EntityManager* entityManager;
	SceneManager* sceneManager;


	const int frameRate = 60;
	const int screenTicks = 1000 / frameRate;

	SString usingVSync;


	mutable bool saveGameRequested = false;
	bool loadGameRequested = false;

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
	Timer frameTime;
	Timer lastSecFrameTime;
	Timer dtTimer;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;

	float secondsSinceStartup;
	float fpsMSeconds;
	float fpsCounter;
	float fps = 0.f;
	float fpsPreUpdate;

	uint frames;
};

extern App* app;

#endif	// __APP_H__