#include "Player.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "Map.h"

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
// Instance reseter
void Player::ResetInstance()
{
	delete instance;
	instance = nullptr;
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
	destroyed = false;
	lifes = 3;

	//
	// Animation pushbacks
	//
	standUpAnim->loop = false;
	standUpAnim->speed = 0.15f;
	for (int i = 0; i < 7; i++)
		standUpAnim->PushBack({ 50 * i, 222, 50, 37 });
	idleAnim->loop = true;
	idleAnim->speed = 0.1f;
	for (int i = 0; i < 4; i++)
		idleAnim->PushBack({ 50 * i, 0, 50, 37 });
	walkAnim->loop = true;
	walkAnim->speed = 0.2f;
	for (int i = 0; i < 6; i++)
		walkAnim->PushBack({50 * i, 37, 50, 37 });
	jumpAnim->loop = false;
	jumpAnim->speed = 0.2f;
	for (int i = 0; i < 8; i++)
		jumpAnim->PushBack({ 50 * i, 74, 50, 37 });
	fallAnim->loop = true;
	fallAnim->speed = 0.2f;
	for (int i = 0; i < 2; i++)
		fallAnim->PushBack({ 50 * i, 111, 50, 37 });
	deathAnim->loop = false;
	deathAnim->speed = 0.1f;
	for (int i = 0; i < 10; i++)
		deathAnim->PushBack({ 50 * i, 148, 50, 37 });
	shootAnim->loop = true;
	shootAnim->speed = 0.2f;
	for (int i = 0; i < 10; i++)
		shootAnim->PushBack({ 50 * i, 185, 50, 37 });
	crouchAnim->loop = false;
	crouchAnim->speed = 0.2f;
	for (int i = 0; i < 4; i++)
		crouchAnim->PushBack({ 50 * i, 259, 50, 37 });
	godAnim->loop = true;
	godAnim->speed = 0.2f;
	for (int i = 0; i < 4; i++)
		godAnim->PushBack({ 50 * i, 296, 50, 37 });

	//
	// Load Player textures files
	//
	playerTexture = tex->Load("Assets/Textures/Player/player_spritesheet.png");


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
	// Load Player variables
	//
	LoadPlayer();

}
// Destructor
Player::~Player()
{

}


// Checks the intersection between two rectangles
inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}
// Main player Update
bool Player::Update(float dt, Map* map)
{
	if (destroyed == false)
	{
		this->map = map;

		UpdateState();
		UpdateLogic(dt);
	}


	return true;
}
// Control input and states
void Player::UpdateState()
{
	switch (state)
	{
	case APPEAR:
	{
		if (currentAnimation->HasFinished() == true)
		{
			currentAnimation->Reset();
			ChangeState(state, IDLE);
			break;
		}


		break;
	}

	case IDLE:
	{
		if (input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || 
			input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT ||
			input->GetKey(SDL_SCANCODE_A) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			ChangeState(state, MOVE);
			break;
		}

		if (input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(state, JUMP);
			break;
		}

		if (isDying == true || isHit == true)
		{
			ChangeState(state, DYING);
			break;
		}
		if (isWinning == true)
		{
			ChangeState(state, WINNING);
			break;
		}
		if (godMode == true)
		{
			ChangeState(state, GOD_MODE);
			break;
		}


		break;
	}

	case MOVE:
	{
		if (input->GetKey(SDL_SCANCODE_D) != KEY_DOWN &&
			input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT &&
			input->GetKey(SDL_SCANCODE_A) != KEY_DOWN &&
			input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		{
			ChangeState(state, IDLE);
			break;
		}

		if (input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(state, JUMP);
			break;
		}

		if (isDying == true || isHit == true)
		{
			ChangeState(state, DYING);
			break;
		}
		if (isWinning == true)
		{
			ChangeState(state, WINNING);
			break;
		}
		if (godMode == true)
		{
			ChangeState(state, GOD_MODE);
			break;
		}


		break;
	}

	case JUMP:
	{
		if (isJumping == false)
		{
			if (input->GetKey(SDL_SCANCODE_D) == KEY_DOWN ||
				input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT ||
				input->GetKey(SDL_SCANCODE_A) == KEY_DOWN ||
				input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				ChangeState(state, MOVE);
				break;
			}

			ChangeState(state, IDLE);
			break;
		}

		if (isDying == true || isHit == true)
		{
			ChangeState(state, DYING);
			break;
		}
		if (isWinning == true)
		{
			ChangeState(state, WINNING);
			break;
		}
		if (godMode == true)
		{
			ChangeState(state, GOD_MODE);
			break;
		}


		break;
	}

	case GOD_MODE:
	{
		if (godMode == false)
		{
			ChangeState(state, IDLE);
			break;
		}

		break;
	}

	case WINNING:
	{
		if (isWinning == false)
		{
			ChangeState(state, IDLE);
		}


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
	case APPEAR:
	{
		currentAnimation->Update();


		break;
	}

	case IDLE:
	{
		ControlFloorCollisionWhenFalling();
		if (isFalling == true)
		{
			currentAnimation = fallAnim;
			acceleration.y = GRAVITY;
		}
		else
		{
			currentAnimation = idleAnim;
			velocity = { 0.0f, 0.0f };
		}


		// Update idle animation
		currentAnimation->Update();


		break;
	}

	case MOVE:
	{
		// Move right control
		if (input->GetKey(SDL_SCANCODE_D) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			horizontalDirection = 1; // Looks right

			MovingRightLogic();
		}
		// Move left control
		if (input->GetKey(SDL_SCANCODE_A) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			horizontalDirection = -1; // Looks left

			MovingLeftLogic();
		}

		// Give velocity to the player
		if (canMoveHorizontally == true)
		{
			velocity.x = PLAYER_SPEED * horizontalDirection * dt;
		}
		else
		{
			velocity.x = 0.0f;
		}
		if (isFalling == true)
		{
			currentAnimation = fallAnim;
			acceleration.y = GRAVITY;
		}
		else
		{
			velocity.y = 0.0f;
		}

		// Update walking animation
		currentAnimation->Update();


		break;
	}

	case JUMP:
	{
		// Allow double jump
		if (canDoubleJump == true && input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			isDoubleJumping = true;
		}

		// Control the jump
		Jump(dt);

		// Check for floor
		if (isFalling == true)
		{
			ControlFloorCollisionWhenFalling();
		}

		// Move right control
		if (input->GetKey(SDL_SCANCODE_D) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			horizontalDirection = 1; // Looks right

			MovingRightLogic();

			if (canMoveHorizontally == true)
			{
				velocity.x = PLAYER_SPEED * horizontalDirection * dt;
			}
			else
			{
				velocity.x = 0.0f;
			}
		}
		// Move left control
		if (input->GetKey(SDL_SCANCODE_A) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			horizontalDirection = -1; // Looks left

			MovingLeftLogic();

			if (canMoveHorizontally == true)
			{
				velocity.x = PLAYER_SPEED * horizontalDirection * dt;
			}
			else
			{
				velocity.x = 0.0f;
			}
		}

		if (input->GetKey(SDL_SCANCODE_D) != KEY_DOWN &&
			input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT &&
			input->GetKey(SDL_SCANCODE_A) != KEY_DOWN &&
			input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		{
			velocity.x = 0.0f;
		}
		
		// Update the jump animation
		currentAnimation->Update();


		break;
	}

	case GOD_MODE:
	{
		// Move freely when player is in god mode
		if (input->GetKey(SDL_SCANCODE_W) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			velocity.y = -3* PLAYER_SPEED * dt;
		}
		if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			velocity.y = 3 * PLAYER_SPEED * dt;
		}
		if (input->GetKey(SDL_SCANCODE_W) != KEY_DOWN &&
			input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT &&
			input->GetKey(SDL_SCANCODE_S) != KEY_DOWN &&
			input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
		{
			velocity.y = 0.0f;
		}
		if (input->GetKey(SDL_SCANCODE_D) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			velocity.x = 3 * PLAYER_SPEED * dt;
		}
		if (input->GetKey(SDL_SCANCODE_A) == KEY_DOWN ||
			input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			velocity.x = -3 * PLAYER_SPEED * dt;
		}
		if (input->GetKey(SDL_SCANCODE_D) != KEY_DOWN &&
			input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT &&
			input->GetKey(SDL_SCANCODE_A) != KEY_DOWN &&
			input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		{
			velocity.x = 0.0f;
		}

		// Update God Animation
		currentAnimation->Update();


		break;
	}

	case WINNING:
	{
		currentAnimation->Update();

		if (currentAnimation->HasFinished() == true)
		{
			isWinning = false;

			changeLevel = true;
		}


		break;
	}

	case DYING:
	{
		currentAnimation->Update();

		if (currentAnimation->HasFinished() == true)
		{
			--lifes;

			isDying = false;

			if (lifes > 0)
			{
				LoadPlayer();
			}
			else
			{
				destroyed = true;
			}

		}
		

		break;
	}


	default:
		break;
	}



	//
	// Position update
	//
	velocity.x = velocity.x + (acceleration.x * dt);
	velocity.y = velocity.y + (acceleration.y * dt);
	position.x += velocity.x;
	position.y += velocity.y;

	//
	// Update Camera Position
	//
	// TODO: Fix camera position

}
// Control what happens when the State is changed
void Player::ChangeState(PlayerState previousState, PlayerState newState)
{
	switch (newState)
	{
	case APPEAR:
	{
		currentAnimation = standUpAnim;


		break;
	}

	case IDLE:
	{
 		currentAnimation = idleAnim;
		acceleration = { 0.0f, 0.0f };


		break;
	}

	case MOVE:
	{
		currentAnimation = walkAnim;
		acceleration = { 0.0f,0.0f };


		break;
	}

	case JUMP:
	{
		currentAnimation = jumpAnim;
		currentAnimation->Reset();

		audioManager->PlayFX(jumpFX);


		break;
	}

	case GOD_MODE:
	{
		currentAnimation = godAnim;
		acceleration = { 0.0f, 0.0f };
		velocity = { 0.0f, 0.0f };


		break;
	}

	case WINNING:
	{
		currentAnimation = crouchAnim;
		currentAnimation->Reset();
		acceleration = { 0.0f, 0.0f };
		velocity = { 0.0f, 0.0f };

		if (playFX == true)
		{
			audioManager->PlayFX(nextLevelFX);
			playFX = false;
		}


		break;
	}

	case DYING:
	{
		currentAnimation = deathAnim;
		currentAnimation->Reset();
		acceleration = { 0.0f, 0.0f };
		velocity = { 0.0f, 0.0f };

		if (playFX == true)
		{
			audioManager->PlayFX(oofFX);
			playFX = false;
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
	if (destroyed == false)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();


		// Player draw when looking right
		if (horizontalDirection == 1)
			render->DrawTexture(playerTexture, (int)position.x - 19, (int)position.y - 9, &rect);

		// Player draw when looking left
		if (horizontalDirection == -1)
			render->DrawFlippedTexture(playerTexture, (int)position.x - 19, (int)position.y - 9, &rect);
	}


	return true;
}
void Player::DrawColliders()
{
	if (destroyed == false)
	{
		render->DrawRectangle(GetRect(), 0, 255, 0, 100);
	}
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


// Load the player's variables and flags
void Player::LoadPlayer()
{
	position.x = savedPos.x;
	position.y = savedPos.y;

	//
	// Set current animation
	//
	currentAnimation = standUpAnim;

	//
	// Set Flags and Variables
	//
	active = true;
	height = PLAYER_HEIGHT;
	width = PLAYER_WIDTH;
	velocity = { 0.0f, 0.0f };
	acceleration = { 0.0f, 0.0f };
	horizontalDirection = 1;
	canMoveHorizontally = true;
	isFalling = false;
	canJump = true;
	canDoubleJump = false;
	isJumping = false;
	isDoubleJumping = false;
	isWinning = false;
	changeLevel = false;
	isDying = false;
	isHit = false;
	playFX = true;
	destroyed = false;
	state = APPEAR;
}
// Load the player's state
bool Player::LoadState(pugi::xml_node& playerNode)
{
	// Load position
	savedPos.x = playerNode.child("position").attribute("position_x").as_float();
	savedPos.y = playerNode.child("position").attribute("position_y").as_float();

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

	case PlayerState::MOVE:
		if (state == GOD_MODE)
		{
			godMode = false;
		}
		state = MOVE;
		currentAnimation->Reset();
		currentAnimation = walkAnim;
		break;

	case PlayerState::JUMP:
		if (state == GOD_MODE)
		{
			godMode = false;
		}
		state = JUMP;
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
	currentAnimation->SetCurrentFrame(playerNode.child("state").attribute("current_frame").as_int());
	horizontalDirection = playerNode.child("state").attribute("horizontal_direction").as_int();

	return true;
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

	case MOVE:
		st = 1;
		break;

	case JUMP:
		st = 2;
		break;

	case GOD_MODE:
		st = 3;
		break;
	}
	player.append_attribute("player_state").set_value(st);
	player.append_attribute("current_frame").set_value(currentAnimation->GetCurrentFrameID());
	player.append_attribute("horizontal_direction").set_value(horizontalDirection);

	player = playerNode.append_child("lifes");
	player.append_attribute("lifes").set_value(lifes);


	return true;
}


// Player's Rectangle Getter
SDL_Rect Player::GetRect()
{
	return { (int)position.x, (int)position.y, width, height };
}



void Player::Jump(float dt)
{
	if (canJump == true)
	{
		isJumping = true;
		canJump = false;
		canDoubleJump = true;
		timeInAir = 0.0f;
	}

	if (isDoubleJumping == true && canDoubleJump == true)
	{
		audioManager->PlayFX(secondJumpFX);

		currentAnimation = jumpAnim;
		currentAnimation->Reset();

		canDoubleJump = false;
		timeInAir = 0.0f;
	}

	if (isJumping == true)
	{
		if (timeInAir < jumpImpulseTime)
		{
			vel.y = jumpImpulseVel;
			if (isDoubleJumping == true)
			{
				vel.y = jumpImpulseVel * 0.8f;
			}
			velocity.y = vel.y * dt;
		}

		acceleration.y = GRAVITY;
	}
	else
	{
		velocity.y = 0.0f;
		acceleration.y = 0.0f;
	}


	if (velocity.y > 0)
	{
		// Change to Falling Animation
		currentAnimation->Reset();
		currentAnimation = fallAnim;
		isFalling = true;
	}

	timeInAir = timeInAir + dt;
}

void Player::MovingRightLogic()
{
	// Check if it's hitting a wall
	MapLayer* layer;
	for (ListItem<MapLayer*>* item = map->data.layers.start; item; item = item->next)
	{
		layer = item->data;
		SString walls = "walls";
		SString colliders = "colliders";
		if (layer->name == walls)
		{
			ControlWallCollisionWhenMovingRight(layer);
		}
		else if (layer->name == colliders && isFalling == false)
		{
			ControlFloorCollisionWhenMovingRight(layer);
		}
		else
		{
			continue;
		}
	}
}
void Player::MovingLeftLogic()
{
	// Check if it's hitting a wall
	MapLayer* layer;
	for (ListItem<MapLayer*>* item = map->data.layers.start; item; item = item->next)
	{
		layer = item->data;
		SString walls = "walls";
		SString colliders = "colliders";
		if (layer->name == walls)
		{
			ControlWallCollisionWhenMovingLeft(layer);
		}
		else if (layer->name == colliders && isFalling == false)
		{
			ControlFloorCollisionWhenMovingLeft(layer);
		}
		else
		{
			continue;
		}
	}
}

void Player::ControlWallCollisionWhenMovingRight(MapLayer* layer)
{
	iPoint playerTile = map->WorldToMap((int)position.x, (int)position.y + (PLAYER_HEIGHT - TILE_SIZE));
	iPoint perfectPlayerTile = map->MapToWorld(playerTile.x, playerTile.y);
	int tileID = layer->Get(playerTile.x + 1, playerTile.y);
	if ((tileID == 4) && (position.x >= perfectPlayerTile.x + (TILE_SIZE - PLAYER_WIDTH)))
	{
		position.x = perfectPlayerTile.x + (TILE_SIZE - PLAYER_WIDTH);
		canMoveHorizontally = false;
	}
	else
	{
		canMoveHorizontally = true;
	}
}
void Player::ControlWallCollisionWhenMovingLeft(MapLayer* layer)
{
	iPoint playerTile = map->WorldToMap((int)position.x, (int)position.y + (PLAYER_HEIGHT - TILE_SIZE));
	iPoint perfectPlayerTile = map->MapToWorld(playerTile.x, playerTile.y);
	int tileID = layer->Get(playerTile.x - 1, playerTile.y);
	if (tileID == 4 && position.x <= perfectPlayerTile.x + 4)
	{
		position.x = perfectPlayerTile.x + 1;
		canMoveHorizontally = false;
	}
	else
	{
		canMoveHorizontally = true;
	}
}
void Player::ControlFloorCollisionWhenMovingRight(MapLayer* layer)
{
	iPoint playerTile = map->WorldToMap((int)position.x, (int)position.y + (PLAYER_HEIGHT - TILE_SIZE));
	iPoint perfectPlayerTile = map->MapToWorld(playerTile.x, playerTile.y);
	int tileID = layer->Get(playerTile.x + 1, playerTile.y + 1);
	if (tileID == 0 && position.x >= perfectPlayerTile.x + TILE_SIZE - 3)
	{
		isFalling = true;
	}
	else
	{
		isFalling = false;
	}
}
void Player::ControlFloorCollisionWhenMovingLeft(MapLayer* layer)
{
	iPoint playerTile = map->WorldToMap((int)position.x, (int)position.y + (PLAYER_HEIGHT - TILE_SIZE));
	iPoint perfectPlayerTile = map->MapToWorld(playerTile.x, playerTile.y);
	int tileID = layer->Get(playerTile.x, playerTile.y + 1);
	if (tileID == 0 && position.x < perfectPlayerTile.x + 5)
	{
		isFalling = true;
	}
	else
	{
		isFalling = false;
	}
}
void Player::ControlFloorCollisionWhenFalling()
{
	MapLayer* layer;
	for (ListItem<MapLayer*>* item = map->data.layers.start; item; item = item->next)
	{
		layer = item->data;
		SString colliders = "colliders";
		if (layer->name == colliders)
		{
			iPoint playerTile = map->WorldToMap((int)position.x, (int)position.y + (PLAYER_HEIGHT - TILE_SIZE));
			iPoint perfectNextTile = map->MapToWorld(playerTile.x, playerTile.y + 1);
			iPoint rightSideTile = map->WorldToMap((int)position.x + width, (int)position.y + (PLAYER_HEIGHT - TILE_SIZE));
			int tileID = layer->Get(playerTile.x, playerTile.y + 1);
			int rightTileID = layer->Get(rightSideTile.x, rightSideTile.y + 1);
			if ((tileID == 1 && position.y + height > perfectNextTile.y - 5) ||
				(playerTile != rightSideTile && rightTileID == 1))
			{
				velocity.y = 0.0f;
				position.y = perfectNextTile.y - height;
				canJump = true;
				isJumping = false;
				canDoubleJump = false;
				isDoubleJumping = false;
				isFalling = false;
			}
			else
			{
				isJumping = true;
			}
		}
		else
		{
			continue;
		}
	}
}