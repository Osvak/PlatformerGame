#include "Player.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"

#include "Collider.h" // TODO: remove

#include "Log.h"
#include "Defs.h"

#include "SDL/include/SDL_rect.h"

#include <stdio.h>



Player* Player::instance = nullptr;

// Instance creator
Player* Player::GetInstance(Input* input, Render* render, Textures* tex, AudioManager* audioManager)
{
	if (instance == nullptr)
	{
		LOG("Creating Player Instance");
		instance = new Player(input, render, tex, audioManager);
	}
	else
	{
		LOG("Returning Instance of Player");
	}

	return instance;
}

// Constructor
Player::Player(Input* input, Render* render, Textures* tex, AudioManager* audioManager) : Entity(EntityType::PLAYER)
{
	LOG("Creating Player Entity");
	
	name.Create("player");


	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;


	
	//
	// Set the lifes to 3
	//
	lifes = 3;

	//
	// Animation pushbacks
	//
	idleAnim->loop = true;
	idleAnim->speed = 0.1f;
	walkAnim->loop = true;
	walkAnim->speed = 0.2f;
	jumpAnim->loop = false;
	jumpAnim->speed = 0.1f;
	fallAnim->loop = true;
	fallAnim->speed = 0.2f;
	deathAnim->loop = false;
	deathAnim->speed = 0.1f;
	for (int i = 0; i < 4; i++)
		idleAnim->PushBack({ 50 * i,0, 50, 37 });
	for (int i = 0; i < 6; i++)
		walkAnim->PushBack({ 50 + (50 * i),37, 50, 37 });
	for (int i = 0; i < 7; i++)
		jumpAnim->PushBack({ 50 * i, 74, 50, 37 });
	jumpAnim->PushBack({ 0, 111, 50, 37 });
	for (int i = 0; i < 2; i++)
		fallAnim->PushBack({ 50 + (50 * i) , 111, 50, 37 });
	for (int i = 0; i < 4; i++)
		deathAnim->PushBack({ 150 + (50 * i), 296, 50, 37 });
	for (int i = 0; i < 6; i++)
		deathAnim->PushBack({ 50 * i, 333, 50, 37 });

	//
	// Load Player textures files
	//
	playerTexture = tex->Load("Assets/Textures/Player/player_sheet.png");


	//
	// Load Player FX files
	//
	jumpFX = audioManager->LoadFX("Assets/Audio/FX/jump.wav");
	audioManager->musicList.Add(&jumpFX);
	secondJumpFX = audioManager->LoadFX("Assets/Audio/FX/second_jump.wav");
	audioManager->musicList.Add(&secondJumpFX);
	oofFX = audioManager->LoadFX("Assets/Audio/FX/oof.wav");
	audioManager->musicList.Add(&oofFX);
	checkpointFX = audioManager->LoadFX("Assets/Audio/FX/checkpoint.wav");
	audioManager->musicList.Add(&checkpointFX);
	nextLevelFX = audioManager->LoadFX("Assets/Audio/FX/next_level.wav");
	audioManager->musicList.Add(&nextLevelFX);

	//
	// Set initial position
	//
	// The initial position is now set on each Level

	//
	// Create Player collider
	//
	//playerCollider = collisions->AddCollider({ (int)position.x , (int)position.y, PLAYER_SIZE, PLAYER_SIZE }, Collider::ColliderType::PLAYER, this);
	//cameraCollider = collisions->AddCollider({ (int)position.x, (int)position.y - (TILE_SIZE * 4), TILE_SIZE * 6, TILE_SIZE * 5 }, Collider::ColliderType::CAMERA_WINDOW, this);

	//
	// Set current animation
	//
	currentAnimation = idleAnim;

	//
	// Set Flags and Variables
	//
	active = true;
	height = PLAYER_SIZE;
	width = PLAYER_SIZE;
	velocity = { 0.0f, 0.0f };
	acceleration = { 0.0f, 0.0f };
	horizontalDirection = 1;
	isTouchingGround = true;
	isTouchingWall = false;
	isJumping = false;
	isWinning = false;
	isDying = false;
	canDoubleJump = false;
	isDoubleJumping = false;
	playFX = true;
	//checkpointPos = { TILE_SIZE * 9, TILE_SIZE * 16 };
	state = IDLE;
	destroyed = false;
}
// Destructor
Player::~Player()
{

}


// Main player Update
bool Player::Update(float dt)
{
	UpdateState();
	UpdateLogic(dt);

	return true;
}
// Control input and states
void Player::UpdateState()
{
	switch (state)
	{
	case IDLE:
	{
		if (input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || 
			input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			ChangeState(state, MOVE_RIGHT);
			break;
		}

		if (input->GetKey(SDL_SCANCODE_A) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			ChangeState(state, MOVE_LEFT);
			break;
		}

		if (input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(state, JUMP);
			break;
		}
		
		if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			ChangeState(state, GOD_MODE);
			break;
		}

		if (isWinning == true)
		{
			ChangeState(state, WINNING);
			break;
		}

		if (isDying == true)
		{
			ChangeState(state, DYING);
			break;
		}

		break;
	}

	case MOVE_RIGHT:
	{
		if (input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(state, JUMP);
			break;
		}

		if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			ChangeState(state, MOVE_LEFT);
			break;
		}

		if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			ChangeState(state, MOVE_RIGHT);
			break;
		}

		if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			ChangeState(state, GOD_MODE);
			break;
		}

		if (isWinning == true)
		{
			ChangeState(state, WINNING);
			break;
		}

		if (isDying == true)
		{
			ChangeState(state, DYING);
			break;
		}

		ChangeState(state, IDLE);

		break;
	}

	case MOVE_LEFT:
	{
		if (input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(state, JUMP);
			break;
		}

		if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			ChangeState(state, MOVE_RIGHT);
			break;
		}

		if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			ChangeState(state, MOVE_LEFT);
			break;
		}
		
		if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			ChangeState(state, GOD_MODE);
			break;
		}

		if (isWinning == true)
		{
			ChangeState(state, WINNING);
			break;
		}


		if (isDying == true)
		{
			ChangeState(state, DYING);
			break;
		}

		ChangeState(state, IDLE);

		break;
	}

	case JUMP:
	{
		if (isTouchingGround == true)
		{
			if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				ChangeState(state, MOVE_RIGHT);
				break;
			}

			if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				ChangeState(state, MOVE_LEFT);
				break;
			}

			if (isWinning == true)
			{
				ChangeState(state, WINNING);
				break;
			}

			if (isDying == true)
			{
				ChangeState(state, DYING);
				break;
			}

			ChangeState(state, IDLE);
		}

		if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			ChangeState(state, GOD_MODE);
			break;
		}

		if (isWinning == true)
		{
			ChangeState(state, WINNING);
			break;
		}

		if (isDying == true)
		{
			ChangeState(state, DYING);
			break;
		}

		break;
	}

	case GOD_MODE:
	{
		if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			//playerCollider->type = Collider::ColliderType::PLAYER;
			
			ChangeState(state, prevState);
		}
		break;
	}

	case WINNING:
	{
		// Nothing to do here
		break;
	}

	case DYING:
	{
		if (isDying == false)
		{
			ChangeState(state, IDLE);
		}
		break;
	}

	
	default:
		break;
	}
}
// Control what each state does
void Player::UpdateLogic(float dt)
{
	switch (state)
	{
	case IDLE:
	{
		velocity.x = 0.0f;
		velocity.y = 0.0f;
		acceleration.x = 0.0f;
		acceleration.y = 0.0f;

		/*while (((position.y + PLAYER_SIZE) > rect.y) && (isTouchingGround == true))
		{
			--position.y;
		}*/

		if (wallCollisionFromLeft == true)
		{
			while ((position.x + PLAYER_SIZE) >= rect.x)
			{
				--position.x;
			}
		}
		if (wallCollisionFromRight == true)
		{
			while (position.x <= (rect.x + rect.w))
			{
				++position.x;
			}
		}

		currentAnimation->Update();

		break;
	}

	case MOVE_RIGHT:
	{
		/*while (((position.y + PLAYER_SIZE) > rect.y) && (isTouchingGround == true))
		{
			--position.y;
		}*/

		if (wallCollisionFromLeft == false)
		{
			velocity.x = PLAYER_SPEED * dt;
		}
		else
		{
			--position.x;
		}

		if (isTouchingGround == true && input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			velocity.x = velocity.x * 2;
			walkAnim->speed = 7.5f * dt;
		}

		walkAnim->speed = 4.0f * dt;

		currentAnimation->Update();

		break;
	}

	case MOVE_LEFT:
	{
		/*while (((position.y + PLAYER_SIZE) > rect.y) && (isTouchingGround == true))
		{
			--position.y;
		}*/

		if (isTouchingWall == false)
		{
			velocity.x = -PLAYER_SPEED * dt;
		}
		else
		{
			velocity.x = 0;
		}

		if (isTouchingGround == true && input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			velocity.x = velocity.x * 2;
			walkAnim->speed = 7.5f * dt;
		}

		walkAnim->speed = 4.0f * dt;

		currentAnimation->Update();

		break;
	}

	case JUMP:
	{
		if ((wallCollisionFromLeft == true) && (verticalDirection != 0))
		{
			while ((position.x + PLAYER_SIZE) > rect.x)
			{
				--position.x;
			}
			fallStraight = true;
		}
		if ((wallCollisionFromRight == true) && (verticalDirection != 0))
		{
			while (position.x < (rect.x + rect.w))
			{
				++position.x;
			}
			fallStraight = true;
		}

		if ((input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) && (fallStraight == false))
		{
			horizontalDirection = 1;
			velocity.x = PLAYER_SPEED * dt;
		}
		if ((input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (fallStraight == false))
		{
			horizontalDirection = 1;
			velocity.x = PLAYER_SPEED * dt;
		}
		if ((input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) && (fallStraight == false))
		{
			horizontalDirection = -1;
			velocity.x = -PLAYER_SPEED * dt;
		}
		if ((input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (fallStraight == false))
		{
			horizontalDirection = -1;
			velocity.x = -PLAYER_SPEED * dt;
		}
		if ((input->GetKey(SDL_SCANCODE_D) != KEY_DOWN) &&
			(input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT) &&
			(input->GetKey(SDL_SCANCODE_A) != KEY_DOWN) &&
			(input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT))
		{
			velocity.x = 0.0f;
		}


		if ((canDoubleJump == true) && (input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN))
		{
			isDoubleJumping = true;

			audioManager->PlayFX(secondJumpFX);
		}

		if (isDying == false)
		{
			Jump(dt);
		}
		currentAnimation->Update();

		break;
	}

	case GOD_MODE:
	{
		if ((input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) ||
			(input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT))
		{
			velocity.y = -PLAYER_SPEED * GOD_MODE_MULT * dt;
		}
		if ((input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) ||
			(input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT))
		{
			velocity.y = PLAYER_SPEED * GOD_MODE_MULT * dt;
		}
		if ((input->GetKey(SDL_SCANCODE_W) != KEY_DOWN) &&
			(input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT) &&
			(input->GetKey(SDL_SCANCODE_S) != KEY_DOWN) &&
			(input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT))
		{
			velocity.y = 0 * dt;
		}

		if ((input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) ||
			(input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
		{
			velocity.x = -PLAYER_SPEED * GOD_MODE_MULT * dt;
		}
		if ((input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) ||
			(input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
		{
			velocity.x = PLAYER_SPEED * GOD_MODE_MULT * dt;
		}
		if ((input->GetKey(SDL_SCANCODE_A) != KEY_DOWN) &&
			(input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT) &&
			(input->GetKey(SDL_SCANCODE_D) != KEY_DOWN) &&
			(input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT))
		{
			velocity.x = 0 * dt;
		}

		break;
	}

	case WINNING:
	{
		// Nothing to do here
		break;
	}

	case DYING:
	{
		currentAnimation->Update();

		isJumping = false;
		isTouchingGround = true;
		
		if (lifes > 0)
		{
			//position = checkpointPos;
			//cameraCollider->SetPos(checkpointPos.x, checkpointPos.y - TILE_SIZE * 4);
		}

		

		break;
	}

	default:
		break;
	}


	//
	// Position update
	//
	velocity.y = velocity.y + (acceleration.y * dt);
	if (velocity.y >= MAX_VELOCITY)
	{
		velocity.y = MAX_VELOCITY * dt;
	}
	velocity.x = velocity.x + (acceleration.x * dt);

	position.x += velocity.x;
	position.y += velocity.y;

	/*if (loadPos == true)
	{
		LoadPlayerPosition();
	}*/


	//
	// Flags reset
	//
	wallCollisionFromLeft = false;
	wallCollisionFromRight = false;

	//
	// Update Collider Position
	//
	if (godMode == false)
	{
		//playerCollider->SetPos(position.x, position.y);
	}

	//
	// Update Camera Position
	//
	/*if (level1->freeCamera == false)
	{
		if ((cameraCollider->rect.y) <= map->cameraMaxBottomPosition)
		{
			render->camera.y = -(cameraCollider->rect.y - (TILE_SIZE * 5)) * (int)win->GetScale();
		}
		render->camera.x = -(cameraCollider->rect.x - (TILE_SIZE * 6)) * (int)win->GetScale();
	}*/
	// TODO: Fix camera position
}
// Change the State
void Player::ChangeState(PlayerState previousState, PlayerState newState)
{
	switch (newState)
	{
	case IDLE:
	{
		wallCollisionFromLeft = false;
		wallCollisionFromRight = false;
		verticalDirection = 0;
		currentAnimation = idleAnim;

		velocity = { 0.0f, 0.0f };
		acceleration = { 0.0f, 0.0f };
		break;
	}

	case MOVE_RIGHT:
	{
		currentAnimation = walkAnim;
		
		verticalDirection = 0;
		horizontalDirection = 1;

		isTouchingWall = false;

		break;
	}

	case MOVE_LEFT:
	{
		currentAnimation = walkAnim;
		verticalDirection = 0;
		horizontalDirection = -1;

		isTouchingWall = false;

		break;
	}

	case JUMP:
	{
		timeInAir = 0.0f;

		currentAnimation = jumpAnim;
		currentAnimation->Reset();
		verticalDirection = -1;
		isJumping = true;

		if (previousState == MOVE_RIGHT)
		{
			horizontalDirection = 1;
		}
		else if (previousState == MOVE_LEFT)
		{
			horizontalDirection = -1;
		}

		audioManager->PlayFX(jumpFX);

		break;
	}

	case GOD_MODE:
	{
		//playerCollider->type = Collider::ColliderType::GOD_MODE;

		prevState = previousState;

		velocity.x = 0.0f;
		velocity.y = 0.0f;
		acceleration.x = 0.0f;
		acceleration.y = 0.0f;

		break;
	}

	case WINNING:
	{
		currentAnimation = idleAnim;
		velocity.x = 0.0f;
		velocity.y = 0.0f;
		acceleration.x = 0.0f;
		acceleration.y = 0.0f;

		break;
	}

	case DYING:
	{
		currentAnimation = deathAnim;
		currentAnimation->Reset();
		
		velocity.x = 0.0f;
		velocity.y = 0.0f;
		acceleration.x = 0.0f;
		acceleration.y = 0.0f;


		if (playFX == true)
		{
			audioManager->PlayFX(oofFX);
		}

		break;
	}

	default:
		break;
	}
		

	state = newState;
}


// Post Update
bool Player::Draw()
{
	//
	// Draw Player
	//
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();


		// Player draw when looking right
		if (horizontalDirection == 1)
			render->DrawTexture(playerTexture, (int)position.x - 17, (int)position.y - (rect.h - 17), &rect);

		// Player draw when looking left
		if (horizontalDirection == -1)
			render->DrawFlippedTexture(playerTexture, (int)position.x - 17, (int)position.y - (rect.h - 17), &rect);
	}


	return true;
}


// Clean Up
bool Player::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Unloading Player");

	idleAnim = nullptr;
	walkAnim = nullptr;
	jumpAnim = nullptr;
	fallAnim = nullptr;
	deathAnim = nullptr;

	tex->UnLoad(playerTexture);

	audioManager->UnloadFX(jumpFX);
	audioManager->UnloadFX(secondJumpFX);
	audioManager->UnloadFX(oofFX);
	audioManager->UnloadFX(checkpointFX);
	audioManager->UnloadFX(nextLevelFX);

	active = false;

	return true;
}


// Controls what the player does when it collides with another collider
/*void Player::OnCollision(Collider* c1, Collider* c2)
{
	//
	// Collision control
	//

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::WALL)
	{
		ControlWallCollision(c2);
		rect = c2->rect;
	}
	else
	{
		wallCollisionFromLeft = false;
		wallCollisionFromRight = false;
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::PLATFORM && c2->type != Collider::ColliderType::CAMERA_WINDOW)
	{
		ControlPlatformCollision(c2);
		rect = c2->rect;
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::NEXT_LEVEL)
	{
		audioManager->PlayFX(nextLevelFX, 0);
		isWinning = true;
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::DIE)
	{
		isDying = true;
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::CHECKPOINT)
	{
		audioManager->PlayFX(checkpointFX, 0);
		if (currentScene == LEVEL1)
		{
			level1->Cp1Activation();
		}
		if (currentScene == LEVEL2)
		{
			level2->Cp2Activation();
		}
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::CAMERA_WINDOW)
	{
		ControlCameraMovement(c2);
	}
	if (c1->type == Collider::ColliderType::GOD_MODE && c2->type == Collider::ColliderType::CAMERA_WINDOW)
	{
		ControlCameraMovement(c2);
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::POTION) {
		++lifes;
		potion->isCollected = true;
	}
}*/


// Load the player's state
bool Player::LoadState(pugi::xml_node& playerNode)
{
	// Load position
	savedPos.x = playerNode.child("position").attribute("position_x").as_float();
	savedPos.y = playerNode.child("position").attribute("position_y").as_float();
	loadPos = true;

	// Load velocity
	velocity.x = playerNode.child("velocity").attribute("velocity_x").as_float();
	velocity.y = playerNode.child("velocity").attribute("velocity_y").as_float();

	// Load state
	st = playerNode.child("state").attribute("player_state").as_int();
	switch (st)
	{
	case PlayerState::IDLE:
		if (state == GOD_MODE)
		{
			godMode = false;
		}
		state = IDLE;
		currentAnimation->Reset();
		currentAnimation = idleAnim;
		break;

	case PlayerState::MOVE_RIGHT:
		if (state == GOD_MODE)
		{
			godMode = false;
		}
		state = MOVE_RIGHT;
		currentAnimation->Reset();
		currentAnimation = walkAnim;
		break;

	case PlayerState::MOVE_LEFT:
		if (state == GOD_MODE)
		{
			godMode = false;
		}
		state = MOVE_LEFT;
		currentAnimation->Reset();
		currentAnimation = walkAnim;
		break;

	case PlayerState::JUMP:
		if (state == GOD_MODE)
		{
			godMode = false;
		}
		state = JUMP;
		isTouchingGround = false;
		currentAnimation->Reset();
		if (velocity.y <= 0.0f)
		{
			currentAnimation = jumpAnim;
			currentAnimation->Reset();
		}
		else
		{
			currentAnimation = fallAnim;
			currentAnimation->Reset();
		}
		break;
	case PlayerState::GOD_MODE:
		state = GOD_MODE;
		break;
	}
	horizontalDirection = playerNode.child("state").attribute("horizontal_direction").as_int();

	return true;
}
void Player::LoadPlayerPosition()
{
	position.x = savedPos.x;
	position.y = savedPos.y;

	//cameraCollider->rect.x = cameraCollPos.x;
	//cameraCollider->rect.y = cameraCollPos.y;

	loadPos = false;
}
// Save the player's state
bool Player::SaveState(pugi::xml_node& playerNode) const
{
	pugi::xml_node player = playerNode.append_child("position");
	player.append_attribute("position_x").set_value(position.x);
	player.append_attribute("position_y").set_value(position.y);

	player = playerNode.append_child("velocity");
	player.append_attribute("velocity_x").set_value(velocity.x);
	player.append_attribute("velocity_y").set_value(velocity.y);

	player = playerNode.append_child("state");
	switch (state)
	{
	case IDLE:
		st = 0;
		break;

	case MOVE_RIGHT:
		st = 1;
		break;

	case MOVE_LEFT:
		st = 2;
		break;

	case JUMP:
		st = 3;
		break;

	case GOD_MODE:
		st = 4;
		break;
	}
	player.append_attribute("player_state").set_value(st);
	player.append_attribute("horizontal_direction").set_value(horizontalDirection);

	player = playerNode.append_child("lifes");
	player.append_attribute("lifes").set_value(lifes);


	return true;
}


void Player::Jump(float dt)
{
	// Allow player to double jump
	if (isDoubleJumping == true && canDoubleJump == true)
	{
		currentAnimation->Reset();
		currentAnimation = jumpAnim;
		currentAnimation->Reset();
		timeInAir = 0.0f;
		accel.y = 0.0;
		vel.y = 0.0;
		verticalDirection = -1;
		isTouchingGround = false;
		canDoubleJump = false;
	}

	// Allow player to jump
	if (isTouchingGround == true) {
		timeInAir = 0.0f ;
		accel.y = 0.0;
		vel.y = 0.0;
		isTouchingGround = false;
		canDoubleJump = true;
	}


	if (timeInAir < jumpImpulseTime)
	{
		vel.y = jumpImpulseVel;
		if (isDoubleJumping == true)
		{
			vel.y = jumpImpulseVel * 0.8f;
		}
		velocity.y = vel.y * dt;
	}
	else if (timeInAir < MAX_AIR_TIME)
	{
		accel.y = jumpAccel;
	}
	else
	{
		accel.y = GRAVITY;

	}

	if (velocity.y > 0)
	{
		//Falling anim
		currentAnimation = fallAnim;
		verticalDirection = 1;
	}
	
	
	timeInAir = timeInAir + 2.5f* dt ;

	
	acceleration.y = accel.y * dt;
}
void Player::ControlWallCollision(Collider* c)
{
	if (isJumping == false || isTouchingGround == false)
	{
		if (position.x < (c->rect.x + c->rect.w) &&
			horizontalDirection == -1)
		{
			velocity.x = 0.0f;
			wallCollisionFromRight = true;
		}

		if ((position.x + PLAYER_SIZE) > c->rect.x &&
			horizontalDirection == 1)
		{
			velocity.x = 0.0f;
			wallCollisionFromLeft = true;
		}
	}
}
void Player::ControlPlatformCollision(Collider* c)
{
	fallStraight = false;
	isJumping = false;
	isDoubleJumping = false;
	canDoubleJump = false;

	if ((position.y + PLAYER_SIZE) > c->rect.y)
	{
		velocity.y = 0.0f;
		position.y = (float)c->rect.y - PLAYER_SIZE;
		isTouchingGround = true;
	}
}
void Player::ControlCameraMovement(Collider* c)
{
	if (position.x < c->rect.x)
	{
		c->rect.x -= c->rect.x - (int)position.x;
	}
	if ((position.x + PLAYER_SIZE) > (c->rect.x + c->rect.w))
	{
		c->rect.x += ((int)position.x + PLAYER_SIZE) - (c->rect.x + c->rect.w);
	}
	if (position.y < c->rect.y)
	{
		c->rect.y -= c->rect.y - (int)position.y;
	}
	if ((position.y + PLAYER_SIZE) > (c->rect.y + c->rect.h))
	{
		c->rect.y += ((int)position.y + PLAYER_SIZE) - (c->rect.y + c->rect.h);
	}

	//cameraCollider->rect = c->rect;
	//TODO: Fix Camera Movement

}


SDL_Rect Player::GetRect()
{
	return { (int)position.x, (int)position.y, width, height };
}