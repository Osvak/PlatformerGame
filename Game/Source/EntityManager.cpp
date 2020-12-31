#include "EntityManager.h"

#include "Player.h"
#include "Map.h"
//#include "Enemy.h"
//#include "Item.h"

#include "Defs.h"
#include "Log.h"



EntityManager::EntityManager(Window* win, Input* input, Render* render, Textures* tex, AudioManager* audioManager) : Module()
{
	name.Create("entitymanager");

	this->win = win;
	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
}

// Destructor
EntityManager::~EntityManager()
{
}


// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("EntityManager.Awake(): Loading Entity Manager");
	bool ret = true;

	return ret;
}


bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMSCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}


// Called before quitting
bool EntityManager::CleanUp()
{
	if (!active) return true;

	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->next;
	}

	return true;
}


// Create a new Entity
Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* ret = nullptr;

	// TODO: Do entities switch
	switch (type)
	{
	case EntityType::MAP:
		ret = new Map(tex, win, render);
		break;
	case EntityType::PLAYER:
		ret = new Player(input, render, tex, audioManager);
		break;
		//case EntityType::ENEMY: ret = new Enemy();  break;
		//case EntityType::ITEM: ret = new Item();  break;
	default: break;
	}

	// Created entities are added to the list
	if (ret != nullptr) AddEntity(ret);

	return ret;
}
// Destroy an existing Entity
void EntityManager::DestroyEntity(Entity* entity)
{
	int i = entities.Find(entity);
	entities.At(i)->data->CleanUp();
	RemoveEntity(entity);
}


// Add an Entity to the Entity List
void EntityManager::AddEntity(Entity* entity)
{
	entities.Add(entity);
}
// Remove an Entity from the Entity List
void EntityManager::RemoveEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (entity == item->data)
		{
			entities.Del(item);
			// TODO: Check for memory leaks
		}
	}
}


bool EntityManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		bool ret = true;
		ListItem<Entity*>* item;
		item = entities.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->Update(dt);
			item = item->next;
		}
	}

	return true;
}