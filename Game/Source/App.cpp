#include "App.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Fonts.h"
#include "AudioManager.h"
#include "EntityManager.h"
#include "Pathfinding.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>


// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(pTimer);

	win = new Window();
	input = new Input(win);
	render = new Render(win);
	tex = new Textures(render);
	fonts = new Fonts(render, tex);
	audioManager = new AudioManager();
	pathFinding = PathFinding::GetInstance();
	entityManager = new EntityManager(win, input, render, tex, audioManager, pathFinding);
	sceneManager = new SceneManager(win, input, render, tex, audioManager, entityManager, pathFinding, fonts);
	

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(fonts);
	AddModule(audioManager);
	AddModule(entityManager);
	AddModule(pathFinding);
	AddModule(sceneManager);

	// Render last to swap buffer
	AddModule(render);


	PERF_PEEK(pTimer);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(pTimer);

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
		
		highFrameRate = configApp.child("fps").attribute("highCap").as_int();
		lowFrameRate = configApp.child("fps").attribute("lowCap").as_int();
		highScreenTicks = 1000 / highFrameRate;
		lowScreenTicks = 1000 / lowFrameRate;
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{

			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}


	PERF_PEEK(pTimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	fpsMSeconds = SDL_GetTicks();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(filenameConfig.GetString());

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", filenameConfig.GetString(), result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	secondsSinceStartup = startupTime.ReadSec();
	fpsPreUpdate = SDL_GetTicks();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// Call Load / Save methods

	if (sceneManager->saveGameRequested == true) SaveGame(filenameGame.GetString());
	if (sceneManager->loadGameRequested == true) LoadGame(filenameGame.GetString());

	if (loadConfigRequested == true) LoadGame(filenameConfig.GetString());
	if (saveConfigRequested == true) SaveGame(filenameConfig.GetString());


	uint32 lastFrameMs = 0;
	uint32 framesOnLastUpdate = 0;

	frameCount++;
	float averageFps = frameCount / secondsSinceStartup;

	fpsCounter++;
	float fpsMSecondsAfter = SDL_GetTicks() - fpsPreUpdate;

	if (fpsMSeconds < SDL_GetTicks() - 1000)
	{
		fpsMSeconds = SDL_GetTicks();
		fps = fpsCounter;
		fpsCounter = 0;
	}


	SString title("Lato Viridi | FPS: %.2f | Avg.FPS: %.2f | Last-Frame MS: %.2f | Time since startup: %.3f | Frame Count: %I64u | VSync: %s",
		fps, averageFps, fpsMSecondsAfter, secondsSinceStartup, frameCount, render->usingVSync.GetString());
	
	win->SetTitle(title.GetString());


	if (fpsMSecondsAfter < highScreenTicks)
	{
		if (sceneManager->fpsCapState == true)
		{
			SDL_Delay(highScreenTicks - fpsMSecondsAfter);
		}
		else
		{
			SDL_Delay(lowScreenTicks - fpsMSecondsAfter);
		}
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	dt = dtTimer.ReadSec();
	dtTimer.Start();

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false)
		{
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false)
		{
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false)
		{
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

void App::LoadConfigRequested()
{
	loadConfigRequested = true;
}

// ---------------------------------------
void App::SaveConfigRequested() const
{
	saveConfigRequested = true;
}

// ---------------------------------------
// Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame(SString fileName)
{
	bool ret = false;

	pugi::xml_parse_result result = saveStateFile.load_file(fileName.GetString());


	if (result != NULL)
	{
		LOG("Loading new Game State from %s...", result.description());
		ret = true;

		nodeSaveStateFile = saveStateFile.child("save_state");
		ListItem<Module*>* item = modules.start;
		

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(nodeSaveStateFile.child(item->data->name.GetString()));
			item = item->next;
		}
		saveStateFile.reset();
		if (ret == true)
		{
			LOG("...finished loading");
		}

	}
	else
	{
		LOG("Could not load game state xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}

	
	sceneManager->loadGameRequested = false;

	return ret;
}

// Implement the XML save method for current state
bool App::SaveGame(SString fileName) const
{
	bool ret = true;

	LOG("Saving Game State to savegame.xml.");


	//XML object where data will be stored
	ListItem<Module*>* item = modules.start;
	pugi::xml_document file;
	pugi::xml_node base = file.append_child("save_state");


	while (item != NULL && ret == true)
	{
		pugi::xml_node node = base.append_child(item->data->name.GetString());
		ret = item->data->SaveState(base.child(item->data->name.GetString())); // Call the SaveState of each module
		item = item->next;
	}

	file.save_file("save_game.xml");

	if (ret == true)
	{
		
		LOG("... finished saving");
	}
	
	bool succ = file.save_file("save_game.xml");
	if (succ != true)
	{
		LOG("File save error. pugi error: %d", pugi::status_internal_error);
	}


	sceneManager->saveGameRequested = false;

	return ret;
}