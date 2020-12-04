#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Log.h"
#include "Defs.h"
#include "Animation.h"

#include "SDL/include/SDL_rect.h"


#define PLAYER_SIZE 16
#define PLAYER_SPEED 1.5f
#define GRAVITY 5.0f
#define MAX_VELOCITY 5.0f
#define MAX_AIR_TIME 0.3f

struct SDL_Texture;
struct SDL_Rect;
struct Collider;

// Different states of the player
enum PlayerState
{
	IDLE,
	MOVE_RIGHT,
	MOVE_LEFT,
	JUMP,
};


// Player Class
class Player : public Module
{
public:
	// Constructor
	Player();

	// Destructor
	~Player();


	// Called when the module is activated, does animation pushbacks
	bool Awake(pugi::xml_node&);

	// Called the first frame
	bool Start();


	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt);

	// Checks for inputs (or timers) and changes the player state accordingly
	void UpdateState();

	// Updates the logic of the player depending on the current state
	void UpdateLogic(float dt);

	// Transition from one state to a new one. Changes animations, resets variables,...
	void ChangeState(PlayerState previousState, PlayerState newState);


	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate();

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) const override;


	// Collision callback, called when the player intersects with another collider
	void OnCollision(Collider* c1, Collider* c2) override;


private:

	void Jump(float dt);

public:

	//// Variables ralated to the player /////
	// Position of the player in the map
	fPoint position;
	fPoint velocity = { 0.0f, 0.0f };
	fPoint acceleration = { 0.0f, 0.0f };

	// The state of the player
	PlayerState state = IDLE;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* playerTexture = nullptr;

	// Current animation
	Animation* currentAnimation;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	//Animation* currentAnimation = nullptr;

	// The Player's collider
	Collider* playerCollider = nullptr;

	// Jump handlers
	fPoint accel = { 0.0, 0.0 };
	fPoint vel = { 0.0, 0.0 };
	float timeInAir = 0.0f;
	float jumpImpulseTime = 0.05f;
	float jumpImpulseVel = -5.0f;
	float jumpAccel = 4.0f;

	////////////////////////////////



		///// Player Flags /////
	// Flag to know if the player is moving
	bool movingFlag = false;

	// The horizontal direction where the player is facing -> -1 for LEFT // 1 for RIGHT // 0 for IDLE
	int horizontalDirection = 1;

	// The horizontal direction where the player is facing -> -1 for UP // 1 for DOWN // 0 for IDLE
	int verticalDirection = 0;

	// Flag to know if the player is touching the ground
	bool isTouchingGround = true;

	// Flag to know if the player is jumping
	bool isJumping = false;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;
	///////////////////////




	/////// ANIMATION SETS ////////
	Animation* idleAnim = new Animation();
	Animation* walkAnim = new Animation();
	//Animation* atakAnim = new Animation();
	//Animation* damageAnim = new Animation();
	//Animation* runAnim = new Animation();
	Animation* jumpAnim = new Animation();
	Animation* fallAnim = new Animation();
	///////////////////////////////

};

#endif //!__PLAYER_H__