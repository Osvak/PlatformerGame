#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

#include "Animation.h"
#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"


#define PLAYER_SIZE 16
#define PLAYER_SPEED 1.5f
#define GOD_MODE_MULT 1.5f
#define GRAVITY 5.0f
#define MAX_VELOCITY 5.0f
#define MAX_AIR_TIME 0.3f



class Input;
class Render;
class Textures;
class AudioManager;

struct SDL_Texture;
struct SDL_Rect;
struct Collider;

// Different states of the player
enum PlayerState
{
	IDLE = 0,
	MOVE_RIGHT,
	MOVE_LEFT,
	JUMP,
	GOD_MODE,
	WINNING,
	DYING,
	
	MAX
};


// Player Class
class Player : public Entity
{
public:

	// Constructor
	Player(Input* input, Render* render, Textures* tex, AudioManager* audioManager);
	// Destructor
	virtual ~Player();


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
	bool Draw();


	// Clean up
	bool CleanUp();


	//bool LoadState(pugi::xml_node&) override;
	
	//bool SaveState(pugi::xml_node&) const override;


	// Collision callback, called when the player intersects with another collider
	//void OnCollision(Collider* c1, Collider* c2) override;

private:

	void LoadPlayerPosition();

	void Jump(float dt);
	void ControlWallCollision(Collider* c1);
	void ControlPlatformCollision(Collider* c1);
	void ControlCameraMovement(Collider* c1);

public:

	//// Variables ralated to the player /////
	// Position of the player in the map
	fPoint position;
	fPoint velocity = { 0.0f, 0.0f };
	fPoint acceleration = { 0.0f, 0.0f };

	// The state of the player
	PlayerState state = IDLE;
	PlayerState prevState = IDLE;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* playerTexture = nullptr;

	// The pointer to the Current animation
	Animation* currentAnimation;
	

	// Auxiliar variables
	SDL_Rect rect;
	int st = 0; // current state for save/load
	int sc = 0; // current scene for save/load


	// Player position form Save File
	fPoint savedPos = { 0.0f,0.0f };
	fPoint cameraCollPos = { 0.0f,0.0f };
	bool loadPos = false;

	// Lifes variable
	int lifes = 3;
	////////////////////////////////



	///////// Player Flags /////////
	// The horizontal direction where the player is facing -> -1 for LEFT // 1 for RIGHT // 0 for IDLE
	int horizontalDirection = 1;
	// The horizontal direction where the player is facing -> -1 for UP // 1 for DOWN // 0 for IDLE
	int verticalDirection = 0;

	// Flag to know if the player is touching the ground
	bool isTouchingGround = true;

	bool wallCollisionFromLeft = false;
	bool wallCollisionFromRight = false;
	bool fallStraight = false;

	// Flag to know if the player is jumping
	bool isJumping = false;
	bool canDoubleJump = false;
	bool isDoubleJumping = false;

	// Flog to know if the God Mode is activated
	bool godMode = false;

	// Flag to know if the player is skipping level
	bool isWinning = false;

	// Flag to know if the player is dying
	bool isDying = false;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;
	///////////////////////////////


	/////// ANIMATION SETS ////////
	Animation* idleAnim = new Animation();
	Animation* walkAnim = new Animation();
	//Animation* shootAnim = new Animation();
	//Animation* damageAnim = new Animation();
	Animation* jumpAnim = new Animation();
	Animation* fallAnim = new Animation();
	Animation* deathAnim = new Animation();
	///////////////////////////////


	//////// Sound Effects ////////
	unsigned int jumpFX;
	unsigned int secondJumpFX;
	unsigned int oofFX;
	unsigned int checkpointFX;
	unsigned int nextLevelFX;
	///////////////////////////////

private:

	// Jump handlers
	fPoint accel = { 0.0, 0.0 };
	fPoint vel = { 0.0, 0.0 };
	float timeInAir = 0.0f;
	float jumpImpulseTime = 0.05f;
	float jumpImpulseVel = -4.0f;
	float jumpAccel = 4.0f;

private:

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;

};

#endif //!__PLAYER_H__