#ifndef __ENEMYSKELETON_H__
#define __ENEMYSKELETON_H__

#include "Entity.h"

#include "Animation.h"
#include "Point.h"
#include "SString.h"

#define SKELETON_WIDTH 10
#define SKELETON_HEIGHT 22


class Render;
class Textures;
class AudioManager;

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
	EnemySkeleton(Render* render, Textures* tex, AudioManager* audioManager);
	// Destructor
	virtual ~EnemySkeleton();


	// Called every loop
	bool Update(float dt);

	// Draws the skeleton
	bool Draw();

	// Release memory
	bool CleanUp();


	// Loads the skeleton state
	bool LoadState(pugi::xml_node&) override;
	// Saves the skeleton state
	bool SaveState(pugi::xml_node&) const override;

	
	SDL_Rect GetRect();


public:

	// ----- SKELETON VARIABLES ----- //
	int width = SKELETON_WIDTH, height = SKELETON_HEIGHT;
	fPoint position;
	fPoint velocity = { 0.0f,0.0f };
	fPoint acceleration = { 0.0f,0.0f };
	// Current state of the skeleton state machine
	SkeletonState state = SKELETON_IDLE;
	// Skeleton textures
	SDL_Texture* skeletonTexture = nullptr;
	// ------------------------------ //
	

	// ----- SKELETON FLAGS ----- //
	int horizontalDirection = -1;
	bool destroyed = false;
	// -------------------------- //

private:

	// ----- SKELETON VARIABLES ----- //
	mutable int st = 0;
	// ------------------------------ //


	// ----- ANIMATION SETS ----- //
	Animation* idleAnim = new Animation();
	Animation* walkAnim = new Animation();
	Animation* attackAnim = new Animation();
	Animation* deathAnim = new Animation();
	Animation* currentAnimation = idleAnim;
	//--------------------------- //


	// ----- SOUND EFFECTS ----- //
	unsigned int attackFX;
	unsigned int deathFX;
	// ------------------------- //

public:

	Render* render;
	Textures* tex;
	AudioManager* audioManager;
};

#endif // !__ENEMYSKELETON_H__