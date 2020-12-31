#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Textures.h"
#include "AudioManager.h"

#include "List.h"

class EntityManager : public Module
{
public:

	// Constructor
	EntityManager(Textures* tex);
	// Destructor
	virtual ~EntityManager();


	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called each loop
	bool Update(float dt);

	// Called before quitting
	bool CleanUp(Textures* tex, AudioManager* audioManager);


	// Additional methods
	Entity* CreateEntity(EntityType type);
	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool UpdateAll(float dt, bool doLogic);

public:

	List<Entity*> entities;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

private:
	
	Textures* tex;
};

#endif // __ENTITYMANAGER_H__