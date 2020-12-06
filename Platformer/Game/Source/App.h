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
class Audio;
class Scene;
class Scene2;
class Map;
class Player;
class Collisions;
class SceneLogo;
class SceneTitle;
class SceneWin;
class SceneLose;
class FadeToBlack;

// All the Scenes
enum GameScenes
{
	NONE = 0,
	LOGO,
	TITLE,
	LEVEL1,
	LEVEL2,
	WIN,
	LOSE,
};

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
	Audio* audio;
	Scene* scene;
	Scene2* scene2;
	Map* map;
	Player* player;
	Collisions* collisions;
	SceneLogo* sceneLogo;
	SceneTitle* sceneTitle;
	SceneWin* sceneWin;
	SceneLose* sceneLose;
	FadeToBlack* fadeToBlack;


	GameScenes lastScene = NONE;
	GameScenes currentScene = LOGO;

	const int frameRate = 60;
	const int screenTicks = 1000 / frameRate;

	SString usingVSync;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;


	mutable bool saveGameRequested = false;
	bool loadGameRequested = false;

	mutable bool saveConfigRequested = false;
	bool loadConfigRequested = false;
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