#ifndef __ENEMYGHOST_H__
#define __ENEMYGHOST_H__

#include "Entity.h"

#include "Animation.h"
#include "Point.h"
#include "SString.h"
#include "DynArray.h"

#define GHOST_WIDTH 18
#define GHOST_HEIGHT 34
#define GHOST_SPEED 20.0f

class Render;
class Textures;
class AudioManager;
class PathFinding;
class Player;
class Map;

struct SDL_Texture;
struct SDL_Rect;

enum GhostState
{
	GHOST_IDLE = 0,
	GHOST_AGGRO,
	GHOST_ATTACK,
	GHOST_DEAGGRO,
	GHOST_DYING
};


class EnemyGhost : public Entity
{
public:

	// Constructor
	EnemyGhost(Render* render, Textures* tex, AudioManager* audioManager, PathFinding* pathFinding);
	// Destructor
	virtual ~EnemyGhost();


	// Called every loop
	bool Update(float dt, Player* player, Map* map);
	// Controls and states
	void UpdateState();
	// Controls what each state does
	void UpdateLogic(float dt);
	// Changes the state
	void ChangeState(GhostState previousState, GhostState newState);


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
	int width = GHOST_WIDTH, height = GHOST_HEIGHT;
	fPoint position;
	fPoint velocity = { 0.0f,0.0f };
	// Current state of the skeleton state machine
	GhostState state = GHOST_IDLE;
	// ------------------------------ //


	// ----- SKELETON FLAGS ----- //
	bool isHit = false;
	bool isDestroyed = false;
	// -------------------------- //

private:

	// ----- SKELETON VARIABLES ----- //
	int horizontalDirection = -1;
	int verticalDirection = 0;
	mutable int st = 0;
	// Range variables
	int visionRange = 150;
	// Path variables
	DynArray<iPoint>* path;
	int pathCreated = -1;
	// Ghost textures
	SDL_Texture* ghostTexture = nullptr;
	// ------------------------------ //


	// ----- GHOST FLAGS ----- //
	bool canWalk = true;
	bool aggroFinished = false;
	bool deaggroFinished = false;
	// -------------------------- //


	// ----- ANIMATION SETS ----- //
	Animation* idleAnim = new Animation();
	Animation* aggroAnim = new Animation();
	Animation* attackAnim = new Animation();
	Animation* deaggroAnim = new Animation();
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

#endif // !__ENEMYGHOST_H__