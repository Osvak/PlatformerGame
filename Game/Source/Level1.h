#ifndef __LEVEL1_H__
#define __LEVEL1_H__

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

class Level1 : public Scene
{
public:

	// Constructor
	Level1(Input* input, Render* render, Textures* tex, AudioManager* audioManager, EntityManager* entityManager, PathFinding* pathFinding);
	// Destructor
	virtual ~Level1();


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

	Map* map = nullptr;
	Player* player = nullptr;
	EnemySkeleton* enemySkeleton = nullptr;
	EnemyGhost* enemyGhost = nullptr;
	ItemPotion* itemPotion = nullptr;
	ItemCoin* itemCoin = nullptr;
	Checkpoint* checkpoint1 = nullptr;
	Checkpoint* checkpoint2 = nullptr;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	EntityManager* entityManager;
	PathFinding* pathFinding;
};

#endif // __LEVEL1_H__