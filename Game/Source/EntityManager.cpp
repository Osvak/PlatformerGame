#include "EntityManager.h"

#include "Player.h"
#include "Map.h"
#include "EnemySkeleton.h"
#include "EnemyGhost.h"
#include "ItemPotion.h"
#include "ItemCoin.h"
#include "Checkpoint.h"

#include "Defs.h"
#include "Log.h"



EntityManager::EntityManager(Window* win, Input* input, Render* render, Textures* tex, AudioManager* audioManager, PathFinding* pathFinding) : Module()
{
	name.Create("entityManager");

	this->win = win;
	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->pathFinding = pathFinding;
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
	if (accumulatedTime >= updateMSCycle)
	{
		doLogic = true;
	}

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

	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		entities.At(i)->data->CleanUp();
		entities.Del(entities.At(i));
	}

	entities.Clear();

	return true;
}


// Save/Load entites
bool EntityManager::LoadState(pugi::xml_node& data)
{
	for (unsigned int i = 0; i < entities.Count(); ++i)
	{
		entities.At(i)->data->LoadState(data.child(entities.At(i)->data->name.GetString()));
	}

	return true;
}

bool EntityManager::SaveState(pugi::xml_node& data) const
{
	for (unsigned int i = 0; i < entities.Count(); ++i)
	{
		entities.At(i)->data->SaveState(data.append_child(entities.At(i)->data->name.GetString()));
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
		ret = Player::GetInstance(input, render, tex, audioManager);
		break;
	case EntityType::ENEMY_SKELETON:
		ret = new EnemySkeleton(render, tex, audioManager, pathFinding); 
		break;
	case EntityType::ENEMY_GHOST:
		ret = new EnemyGhost(render, tex, audioManager, pathFinding);
		break;
	case EntityType::ITEM_POTION:
		ret = new ItemPotion(render, tex, audioManager);
		break;
	case EntityType::ITEM_COIN:
		ret = new ItemCoin(render, tex, audioManager);
		break;
	case EntityType::CHECKPOINT:
		ret = new Checkpoint(render, tex, audioManager);
		break;
	default:
		break;
	}

	// Created entities are added to the list
	if (ret != nullptr && entities.Find(ret) == -1)
	{
		AddEntity(ret);
	}

	return ret;
}
// Destroy an existing Entity
void EntityManager::DestroyEntity(Entity* entity)
{
	int i = entities.Find(entity);
	if (i == -1)
	{
		return;
	}
	else
	{
		LOG("Destroying Entity %s", entity->name.GetString());
		entities.At(i)->data->CleanUp();
		RemoveEntity(i);
	}
}


// Add an Entity to the Entity List
void EntityManager::AddEntity(Entity* entity)
{
	entities.Add(entity);
}
// Remove an Entity from the Entity List
void EntityManager::RemoveEntity(int i)
{
	entities.Del(entities.At(i));
}


bool EntityManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		for (unsigned int i = 0; i < entities.Count(); ++i)
		{
			//entities.At(i)->data->Update(dt);
		}
	}

	return true;
}