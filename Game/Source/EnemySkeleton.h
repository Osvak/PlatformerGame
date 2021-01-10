#ifndef __ENEMYSKELETON_H__
#define __ENEMYSKELETON_H__

#include "Entity.h"

#include "Animation.h"
#include "Point.h"
#include "SString.h"
#include "DynArray.h"

#define SKELETON_WIDTH 10
#define SKELETON_HEIGHT 22
#define SKELETON_SPEED 30.0f
#define ATTACK_COOLDOWN 40

class Render;
class Textures;
class AudioManager;
class PathFinding;
class Player;
class Map;

struct SDL_Texture;
struct SDL_Rect;

enum SkeletonState
{
	SKELETON_IDLE = 0,
	SKELETON_MOVE,
	SKELETON_ATTACK,
	SKELETON_DYING
};


class EnemySkeleton : public Entity
{
public:

	// Constructor
	EnemySkeleton(Render* render, Textures* tex, AudioManager* audioManager, PathFinding* pathFinding, uint32 id = 0);
	// Destructor
	virtual ~EnemySkeleton();


	// Called every loop
	bool Update(float dt, Player* player, Map* map);
	// Controls and states
	void UpdateState();
	// Controls what each state does
	void UpdateLogic(float dt);
	// Changes the state
	void ChangeState(SkeletonState previousState, SkeletonState newState);


	// Draws the skeleton
	bool Draw();
	void DrawColliders();

	// Release memory
	bool CleanUp();


	// Loads the skeleton state
	bool LoadState(pugi::xml_node&) override;
	// Saves the skeleton state
	bool SaveState(pugi::xml_node&) const override;

	
	SDL_Rect GetRect();


	bool Radar(fPoint playerPosition, int range);

public:

	// ----- SKELETON VARIABLES ----- //
	int width = SKELETON_WIDTH, height = SKELETON_HEIGHT;
	fPoint position;
	fPoint velocity = { 0.0f,0.0f };
	// Current state of the skeleton state machine
	SkeletonState state = SKELETON_IDLE;
	// ------------------------------ //
	

	// ----- SKELETON FLAGS ----- //
	int horizontalDirection = -1;
	bool isHit = false;
	bool isDestroyed = false;
	// -------------------------- //

private:

	// ----- SKELETON VARIABLES ----- //
	mutable int st = 0;
	// Range variables
	int visionRange = 150;
	int attackRange = 42;
	// Path variables
	DynArray<iPoint>* path;
	int pathCreated = -1;
	// Attack variables
	int attackCooldwon = 0;
	SDL_Rect attackRect = { 0, 0, 0, 0 };
	// Skeleton textures
	SDL_Texture* skeletonTexture = nullptr;
	// ------------------------------ //


	// ----- SKELETON FLAGS ----- //
	bool canWalk = true;
	bool attackFinished = false;
	bool playFX = true;
	// -------------------------- //


	// ----- ANIMATION SETS ----- //
	Animation* idleAnim = new Animation();
	Animation* walkAnim = new Animation();
	Animation* attackAnim = new Animation();
	Animation* deathAnim = new Animation();
	Animation* currentAnimation = idleAnim;
	//--------------------------- //


	// ----- SOUND EFFECTS ----- //
	unsigned int attackFX = 0;
	unsigned int deathFX = 0;
	// ------------------------- //

public:

	Render* render = nullptr;
	Textures* tex = nullptr;
	AudioManager* audioManager = nullptr;
	PathFinding* pathFinding = nullptr;
	Player* player = nullptr;
	Map* map = nullptr;
};

#endif // !__ENEMYSKELETON_H__