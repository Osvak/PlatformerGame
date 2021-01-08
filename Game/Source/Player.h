#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

#include "Animation.h"
#include "Point.h"
#include "SString.h"


#define PLAYER_WIDTH 12
#define PLAYER_HEIGHT 28
#define PLAYER_SPEED 80.0f
#define GOD_MODE_MULT 1.5f
#define GRAVITY 10.0f
#define MAX_VELOCITY 50.0f
#define MAX_AIR_TIME 2.0f



class Input;
class Render;
class Textures;
class AudioManager;
class Map;
class MapLayer;

struct SDL_Texture;
struct SDL_Rect;
struct Collider;

// Different states of the player
enum PlayerState
{
	IDLE = 0,
	MOVE,
	JUMP,
	GOD_MODE,
	WINNING,
	DYING,
	APPEAR,
	
	MAX
};


// Player Class
class Player : public Entity
{
public:

	// Get unique isntance of the class
	static Player* GetInstance(Input* input, Render* render, Textures* tex, AudioManager* audioManager);
	// Destructor
	virtual ~Player();


	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt, Map* map);
	// Checks for inputs (or timers) and changes the player state accordingly
	void UpdateState();
	// Updates the logic of the player depending on the current state
	void UpdateLogic(float dt);
	// Transition from one state to a new one. Changes animations, resets variables,...
	void ChangeState(PlayerState previousState, PlayerState newState);
	

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool Draw();
	void DrawColliders();


	// Clean up
	bool CleanUp();


	bool LoadState(pugi::xml_node&) override;
	bool SaveState(pugi::xml_node&) const override;

	// Player size getter
	SDL_Rect GetRect();


	// Collision callback, called when the player intersects with another collider
	//void OnCollision(Collider* c1, Collider* c2) override;

private:

	// Singleton instance
	static Player* instance;
	// Private Constructor
	Player(Input* input, Render* render, Textures* tex, AudioManager* audioManager);
	// Declare the copy constructor and the assignment operator as 
	// private (or delete them explicitly) to prevent cloning your object
	Player(const Player&);
	Player& operator=(const Player&);


	void LoadPlayerPosition();

	void Jump(float dt);
	void ControlWallCollision(Collider* c1);
	void ControlPlatformCollision(Collider* c1);
	void ControlCameraMovement(Collider* c1);
	void ControlFall(Map* map);


	// ---- PLAYER MOVEMENT METHODS ----- //
	void MovingRightLogic();
	void MovingLeftLogic();
	void ControlWallCollisionWhenMovingRight(MapLayer* layer);
	void ControlWallCollisionWhenMovingLeft(MapLayer* layer);
	void ControlFloorCollisionWhenMovingRight(MapLayer* layer);
	void ControlFloorCollisionWhenMovingLeft(MapLayer* layer);
	void ControlFloorCollisionWhenFalling();
	// ---------------------------------- //

public:

	// ----- PLAYER VARIABLES ----- //
	// Position of the player in the map
	int width = PLAYER_WIDTH, height = PLAYER_HEIGHT;
	fPoint position;
	fPoint velocity = { 0.0f,0.0f };
	fPoint acceleration = { 0.0f,0.0f };
	// The state of the player
	PlayerState state = APPEAR;
	PlayerState prevState = IDLE;
	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* playerTexture = nullptr;
	// The pointer to the Current animation
	Animation* currentAnimation;
	// Auxiliar variables
	SDL_Rect rect;
	mutable int st = 0; // current state for save/load
	mutable int sc = 0; // current scene for save/load
	// Player position form Save File
	fPoint savedPos = { 0.0f,0.0f };
	fPoint cameraCollPos = { 0.0f,0.0f };
	bool loadPos = false;
	// Lifes variable
	int lifes = 3;
	// ---------------------------- //


	// ----- PLAYER FLAGS ----- //
	// The horizontal direction where the player is facing -> -1 for LEFT // 1 for RIGHT // 0 for IDLE
	int horizontalDirection = 1;
	// The horizontal direction where the player is facing -> -1 for UP // 1 for DOWN // 0 for IDLE
	int verticalDirection = 0;
	
	// Flag to know if the player is jumping
	bool isJumping = false;
	
	bool isDoubleJumping = false;
	// Flog to know if the God Mode is activated
	bool godMode = false;
	// Flag to know if the player is skipping level
	bool isWinning = false;
	// Flag to know if the player is dying
	bool isDying = false;
	// Flag to know if the player has been hit
	bool isHit = false;
	// A flag to detect when the player has been destroyed
	bool destroyed = false;
	// ------------------------ //


	//----- ANIMATION SETS ----- //
	Animation* standUpAnim = new Animation();
	Animation* idleAnim = new Animation();
	Animation* walkAnim = new Animation();
	Animation* shootAnim = new Animation();
	//Animation* damageAnim = new Animation();
	Animation* jumpAnim = new Animation();
	Animation* fallAnim = new Animation();
	Animation* deathAnim = new Animation();
	//--------------------------- //


	// ----- SOUND EFFECTS ----- //
	unsigned int jumpFX;
	unsigned int secondJumpFX;
	unsigned int oofFX;
	unsigned int checkpointFX;
	unsigned int nextLevelFX;
	// Sound effects flags
	bool playFX = true;
	// ------------------------- //

private:

	// ----- PLAYER VARIABLES ----- //
	// Jump handlers
	fPoint accel = { 0.0f, 0.0f };
	fPoint vel = { 0.0f, 0.0f };
	float timeInAir = 0.0f;
	float jumpImpulseTime = 0.4f;
	float jumpImpulseVel = -150.0f;
	float jumpAccel = 40.0f;
	// ---------------------------- //


	// ----- PLAYER FLAGS ----- //
	// Movement control flags
	bool canMoveHorizontally = true;
	bool isFalling = false;
	bool canJump = true;
	bool canDoubleJump = false;
	// ------------------------ //

public:

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audioManager;
	Map* map;
};

#endif //!__PLAYER_H__