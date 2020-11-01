#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Log.h"
#include "Defs.h"
#include "SDL/include/SDL_rect.h"


#define PLAYER_SIZE 16

struct SDL_Texture;
struct Collider;

// Different states of the player
enum Player_State
{
	IDLE,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_RIGHT,
	MOVE_LEFT,
};


// Player Class
class Player : public Module
{
public:
	// Constructor
	Player();

	// Destructor
	~Player();


	// Called when the module is activated, loads textures
	bool Awake(pugi::xml_node&);


	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update();

	// Checks for inputs (or timers) and changes the player state accordingly
	void UpdateState();

	// Updates the logic of the player depending on the current state
	void UpdateLogic();

	// Transition from one state to a new one. Changes animations, resets variables,...
	void ChangeState(Player_State previousState, Player_State newState);


	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate();


	// Collision callback, called when the player intersects with another collider
	//void OnCollision(Collider* c1, Collider* c2) override;


public:

	//// Variables ralated to the player /////
	// Position of the player in the map
	iPoint position;

	// The state of the player
	Player_State state = IDLE;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* playerTexture = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	//Animation* currentAnimation = nullptr;

	// The Player's collider
	//Collider* collider = nullptr;

	// Jump handlers
	uint jumpCountdown = 30;
	int jumpDirection = 1;

	// The speed in which we move the player (pixels per frame)
	int speed = 1;

	int lastPos = 0;



	// Player's placeholder square
	SDL_Rect plSquare;
	////////////////////////////////






	///// Player Flags /////
	// Flag to know if the player is moving
	bool movingFlag = false;

	// The horizontal direction where the player is facing -> -1 for LEFT // 1 for RIGHT
	int horizontalDirection = 0;

	// The horizontal direction where the player is facing -> -1 for UP // 1 for DOWN
	int verticalDirection = 0;


	// A flag to detect when the player has been destroyed
	bool destroyed = false;
	/////////////////////




	/////// ANIMATION SETS ////////

	///////////////////////////////

};

#endif //!__PLAYER_H__