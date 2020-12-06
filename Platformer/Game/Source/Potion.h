#ifndef __POTION_H__
#define __POTION_H__

#include "Module.h"

#include "Point.h"

struct SDL_Texture;
struct SDL_Rect;
struct Collider;

class Potion : public Module
{
public:
	// Constructor
	Potion();

	// Destructor
	~Potion();


	// Called when the module is activated, does animation pushbacks
	bool Awake();

	// Called the first frame
	bool Start();

	// Called at the middle of the application loop
	bool Update(float dt);

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate();

	// Clean up
	bool CleanUp();

public:
	//// Variables ralated to the potion /////
	iPoint potionPosition;

	// The potion sprite loaded into an SDL_Texture
	SDL_Texture* potionTexture = nullptr;

	// The Potion's collider
	Collider* potionCollider = nullptr;

	// Auxiliar variables
	SDL_Rect potionRect;

	//// Potion Flags /////

	// Flag to know if the potion has been collected
	bool isCollected = false;

	// Flag to know if the potion is being grabbed
	int potionsActive = 1;
	   
};

#endif // !__POTION_H__
