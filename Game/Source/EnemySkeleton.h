#ifndef __ENEMYSKELETON_H__
#define __ENEMYSKELETON_H__

#include "Entity.h"

#include "Animation.h"
#include "Point.h"
#include "SString.h"



class Render;
class Textures;
class AudioManager;

struct SDL_Texture;
struct SDL_Rect;

enum SkeletonState
{
	SKELETON_IDLE,
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


	bool Update(float dt);

	bool Draw();


	bool CleanUp();


	bool LoadState(pugi::xml_node&) override;
	bool SaveState(pugi::xml_node&) const override;

public:

	// ----- SKELETON VARIABLES ----- //
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