#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"

#include "Entity.h"

#include "List.h"


class Window;
class Input;
class Render;
class Textures;
class AudioManager;

class EntityManager : public Module
{
public:

	// Constructor
	EntityManager(Window* win, Input* input, Render* render, Textures* tex, AudioManager* audioManager);
	// Destructor
	virtual ~EntityManager();


	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called each loop
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();	


	// Additional methods
	Entity* CreateEntity(EntityType type);
	void DestroyEntity(Entity* entity);

	// Update all entities
	bool UpdateAll(float dt, bool doLogic);


private:

	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

public:

	List<Entity*> entities;

	float accumulatedTime = 0.0f;
	float updateMSCycle = 0.0f;
	bool doLogic = false;

private:
	
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
};

#endif // __ENTITYMANAGER_H__