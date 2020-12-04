#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Collisions.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();
	map = new Map();
	player = new Player();
	collisions = new Collisions();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(scene);
	AddModule(map);
	AddModule(player);
	AddModule(collisions);

	// Render last to swap buffer
	AddModule(render);
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

	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
		
		//framerate = configApp.attribute("framerate_cap").as_int(0);
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

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

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

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L02: DONE 1: This is a good place to call Load / Save methods

	if (saveGameRequested == true) SaveGame(filenameGame.GetString());
	if (loadGameRequested == true) LoadGame(filenameGame.GetString());

	if (loadConfigRequested == true) LoadGame(filenameConfig.GetString());
	if (saveConfigRequested == true) SaveGame(filenameConfig.GetString());
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
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

		if(pModule->active == false) {
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

		if(pModule->active == false) {
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

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

void App::LoadConfigRequested()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadConfigRequested = true;
}

// ---------------------------------------
void App::SaveConfigRequested() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveConfigRequested = true;
}

// ---------------------------------------
// L02: DONE 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame(SString filename)
{
	bool ret = false;

	pugi::xml_parse_result result = saveStateFile.load_file(filename.GetString());


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


	loadGameRequested = false;

	return ret;
}

// L02: DONE 7: Implement the xml save method for current state
bool App::SaveGame(SString filename) const
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
		ret = item->data->SaveState(base.child(item->data->name.GetString()));
		item = item->next;
	}

	file.save_file("save_game.xml"); // unknown error with the "fopen" method

	if (ret == true)
	{
		
		LOG("... finished saving");
	}
	
	bool succ = file.save_file("save_game.xml");
	if (succ != true)
	{
		LOG("File save error. pugi error: %d", pugi::status_internal_error);
	}


	saveGameRequested = false;

	return ret;
}