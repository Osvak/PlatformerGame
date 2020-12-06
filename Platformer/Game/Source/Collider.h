#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_rect.h"


#define MAX_LISTENERS 300



class Module;

// Collider struct
struct Collider
{
	// List of Collider Types
	enum ColliderType
	{
		NONE = -1,

		PLAYER,
		WALL,
		PLATFORM,
		DIE,
		NEXT_LEVEL,
		GOD_MODE,
		CHECKPOINT,
		CAMERA_WINDOW,
		POTION,

		MAX
	};


// Methods
public:
	// Constructor
	Collider(SDL_Rect rectangle, ColliderType type, Module* listener = nullptr);

	// Set position of the collider
	void SetPos(int x, int y);

	// Check if two colliders are intersecting
	bool CheckCollision(const SDL_Rect& r) const;

	// Add listener
	void AddListener(Module* listener);


// Variables
public:
	SDL_Rect rect;
	bool pendingToDelete = false;
	ColliderType type;
	Module* listeners[MAX_LISTENERS] = { nullptr };
};

#endif // !__COLLIDER_H__