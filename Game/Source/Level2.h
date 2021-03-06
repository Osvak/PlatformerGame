#ifndef __LEVEL2_H__
#define __LEVEL2_H__

#include "Scene.h"



class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;
class PathFinding;

class Map;
class Player;
class EnemySkeleton;
class EnemyGhost;
class ItemPotion;
class ItemCoin;
class Checkpoint;

class Level2 : public Scene
{
public:

	// Constructor
	Level2(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager, PathFinding* pathfinding);
	// Destructor
	virtual ~Level2();


	// Called before the first frame
	bool Load();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool Draw();


	// Called before quitting
	bool Unload();


	void CollisionLogic();

private:

	void CheckpointLogic();

private:

	Map* map = nullptr;
	Player* player = nullptr;
	EnemySkeleton* enemySkeleton1 = nullptr;
	EnemySkeleton* enemySkeleton2 = nullptr;
	EnemySkeleton* enemySkeleton3 = nullptr;
	EnemyGhost* enemyGhost1 = nullptr;
	EnemyGhost* enemyGhost2 = nullptr;
	ItemPotion* itemPotion = nullptr;
	ItemCoin* itemCoin1 = nullptr;
	ItemCoin* itemCoin2 = nullptr;
	ItemCoin* itemCoin3 = nullptr;
	Checkpoint* checkpoint1 = nullptr;
	Checkpoint* checkpoint2 = nullptr;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	EntityManager* entityManager;
	PathFinding* pathFinding;
};

#endif // __LEVEL2_H__