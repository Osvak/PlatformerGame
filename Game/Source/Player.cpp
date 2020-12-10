#include "Player.h"

#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Map.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Level1.h"
#include "Level2.h"
#include "Potion.h"

#include "Log.h"
#include "Defs.h"


#include "SDL/include/SDL_rect.h"

#include <stdio.h>


// Constructor
Player::Player() : Module()
{
	name.Create("player");
}

// Destructor
Player::~Player()
{

}

// Called when the module is activated
bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");

	bool ret = true;

	//
	// Set the lifes to 3
	//
	lifes = 3;

	//
	// Animation pushbacks
	//
	idleAnim->loop = true;
	idleAnim->speed = 0.2f;
	walkAnim->loop = true;
	walkAnim->speed = 0.2f;
	jumpAnim->loop = false;
	jumpAnim->speed = 0.25f;
	fallAnim->loop = true;
	fallAnim->speed = 0.2f;
	deathAnim->loop = false;
	deathAnim->speed = 0.3f;


	for (int i = 0; i < 4; i++)
		idleAnim->PushBack({ 50 * i,0, 50, 37 });

	for (int i = 0; i < 6; i++)
		walkAnim->PushBack({ 50 + (50 * i),37, 50, 37 });

	for (int i = 0; i < 7; i++)
		jumpAnim->PushBack({ 50 * i, 74, 50, 37 });
		jumpAnim->PushBack({ 0, 111, 50, 37 });

	for (int i = 0; i < 2 ; i++)
		fallAnim->PushBack({ 50 + (50 * i) , 111, 50, 37 });

	for (int i = 0; i < 4; i++)
		deathAnim->PushBack({ 150 + (50 * i), 296, 50, 37 });
	for (int i = 0; i < 6; i++)
		deathAnim->PushBack({ 50 * i, 333, 50, 37 });

	return ret;
}

// Called the first frame
bool Player::Start()
{
	LOG("+++++ Loading player textures");


	//
	// Load Player textures files
	//
	playerTexture = app->tex->Load("Assets/Textures/Player/player_sheet.png");

	//
	// Load Lifes texture files 
	//
	lifesTexture = app->tex->Load("Assets/Textures/Items/heart_sprite.png");

	//
	// Load Player FX files
	//
	jumpFX = app->audio->LoadFX("Assets/Audio/FX/jump.wav");
	app->musicList.Add(&jumpFX);
	secondJumpFX = app->audio->LoadFX("Assets/Audio/FX/second_jump.wav");
	app->musicList.Add(&secondJumpFX);
	oofFX = app->audio->LoadFX("Assets/Audio/FX/oof.wav");
	app->musicList.Add(&oofFX);
	checkpointFX = app->audio->LoadFX("Assets/Audio/FX/checkpoint.wav");
	app->musicList.Add(&checkpointFX);
	nextLevelFX = app->audio->LoadFX("Assets/Audio/FX/next_level.wav");

	//
	// Set initial position
	//
	if (app->currentScene == LEVEL1)
	{
		position.x = TILE_SIZE * 8; // Tile size * Tile ammount
		position.y = TILE_SIZE * 16; // Tile size * Tile ammount
	}
	if (app->currentScene == LEVEL2)
	{
		position.x = TILE_SIZE * 3; // Tile size * Tile ammount
		position.y = TILE_SIZE * 24; // Tile size * Tile ammount
	}


	//
	// Create Player collider
	//
	playerCollider = app->collisions->AddCollider({ (int)position.x , (int)position.y, PLAYER_SIZE, PLAYER_SIZE }, Collider::ColliderType::PLAYER, this);
	cameraCollider = app->collisions->AddCollider({ (int)position.x, (int)position.y - (TILE_SIZE * 4), TILE_SIZE * 6, TILE_SIZE * 5 }, Collider::ColliderType::CAMERA_WINDOW, this);


	//
	// Set current animation
	//
	currentAnimation = idleAnim;


	//
	// Set Flags and Variables
	//
	active = true;
	velocity = { 0.0f, 0.0f };
	acceleration = { 0.0f, 0.0f };
	horizontalDirection = 1;
	isTouchingGround = true;
	isJumping = false;
	isWinning = false;
	isDying = false;
	savedPos = { TILE_SIZE * 8, TILE_SIZE * 16 };
	state = IDLE;
	
	if (app->lastScene == TITLE)
	{
		destroyed = false;
	}
	return true;
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
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			ChangeState(state, MOVE_RIGHT);
			break;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			ChangeState(state, MOVE_LEFT);
			break;
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(state, JUMP);
			break;
		}
		
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
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
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(state, JUMP);
			break;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			ChangeState(state, MOVE_LEFT);
			break;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			ChangeState(state, MOVE_RIGHT);
			break;
		}

		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
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
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(state, JUMP);
			break;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			ChangeState(state, MOVE_RIGHT);
			break;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			ChangeState(state, MOVE_LEFT);
			break;
		}
		
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
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
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				ChangeState(state, MOVE_RIGHT);
				break;
			}

			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
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

		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
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
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			playerCollider->type = Collider::ColliderType::PLAYER;
			
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
	dt *= 2;

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
			velocity.x = PLAYER_SPEED;
		}
		else
		{
			--position.x;
		}

		if (isTouchingGround == true && app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			velocity.x = velocity.x * 2;
			walkAnim->speed = 0.8f;
		}

		walkAnim->speed = 0.2f;

		currentAnimation->Update();

		break;
	}

	case MOVE_LEFT:
	{
		/*while (((position.y + PLAYER_SIZE) > rect.y) && (isTouchingGround == true))
		{
			--position.y;
		}*/

		if (wallCollisionFromRight == false)
		{
			velocity.x = -PLAYER_SPEED;
		}
		else
		{
			++position.x;
		}

		if (isTouchingGround == true && app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			velocity.x = velocity.x * 2;
			walkAnim->speed = 0.8f;
		}

		walkAnim->speed = 0.2f;

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

		if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) && (fallStraight == false))
		{
			horizontalDirection = 1;
			velocity.x = PLAYER_SPEED;
		}
		if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (fallStraight == false))
		{
			horizontalDirection = 1;
			velocity.x = PLAYER_SPEED;
		}
		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) && (fallStraight == false))
		{
			horizontalDirection = -1;
			velocity.x = -PLAYER_SPEED;
		}
		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (fallStraight == false))
		{
			horizontalDirection = -1;
			velocity.x = -PLAYER_SPEED;
		}
		if ((app->input->GetKey(SDL_SCANCODE_D) != KEY_DOWN) &&
			(app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT) &&
			(app->input->GetKey(SDL_SCANCODE_A) != KEY_DOWN) &&
			(app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT))
		{
			velocity.x = 0.0f;
		}


		if ((canDoubleJump == true) && (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN))
		{
			isDoubleJumping = true;

			app->audio->PlayFX(secondJumpFX);
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
		if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) ||
			(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT))
		{
			velocity.y = -PLAYER_SPEED * GOD_MODE_MULT;
		}
		if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) ||
			(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT))
		{
			velocity.y = PLAYER_SPEED * GOD_MODE_MULT;
		}
		if ((app->input->GetKey(SDL_SCANCODE_W) != KEY_DOWN) &&
			(app->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT) &&
			(app->input->GetKey(SDL_SCANCODE_S) != KEY_DOWN) &&
			(app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT))
		{
			velocity.y = 0;
		}

		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) ||
			(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
		{
			velocity.x = -PLAYER_SPEED * GOD_MODE_MULT;
		}
		if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) ||
			(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
		{
			velocity.x = PLAYER_SPEED * GOD_MODE_MULT;
		}
		if ((app->input->GetKey(SDL_SCANCODE_A) != KEY_DOWN) &&
			(app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT) &&
			(app->input->GetKey(SDL_SCANCODE_D) != KEY_DOWN) &&
			(app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT))
		{
			velocity.x = 0;
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

		if (currentAnimation->HasFinished() == true)
		{
				isDying = false;

				isJumping = false;
				isTouchingGround = true;

				if (lifes > 0)
				{
					position = savedPos;
					cameraCollider->SetPos(savedPos.x, savedPos.y - TILE_SIZE * 4);
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
	velocity.y = velocity.y + (acceleration.y * dt);
	if (velocity.y >= MAX_VELOCITY)
	{
		velocity.y = MAX_VELOCITY;
	}
	velocity.x = velocity.x + (acceleration.x * dt);

	position.x += velocity.x /** dt*/;
	position.y += velocity.y /** dt*/;


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
		playerCollider->SetPos(position.x, position.y);
	}

	//
	// Update Camera Position
	//
	if (app->level1->freeCamera == false)
	{
		app->render->camera.x = -(cameraCollider->rect.x - (TILE_SIZE * 6)) * (int)app->win->GetScale();
		app->render->camera.y = -(cameraCollider->rect.y - (TILE_SIZE * 5)) * (int)app->win->GetScale();
	}

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


		break;
	}

	case MOVE_LEFT:
	{
		currentAnimation = walkAnim;
		verticalDirection = 0;
		horizontalDirection = -1;

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

		app->audio->PlayFX(jumpFX);

		break;
	}

	case GOD_MODE:
	{
		playerCollider->type = Collider::ColliderType::GOD_MODE;

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

		--lifes;

		app->audio->PlayFX(oofFX);

		break;
	}

	default:
		break;
	}
		

	state = newState;
}

// Post Update
bool Player::PostUpdate()
{
	//
	// Draw Player
	//
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();


		// Player draw when walking right
		if (horizontalDirection == 1)
			app->render->DrawTexture(playerTexture, position.x - 17, position.y - (rect.h - 17), &rect);

		// Player draw when walking right
		if (horizontalDirection == -1)
			app->render->DrawFlippedTexture(playerTexture, position.x - 17, position.y - (rect.h - 17), &rect);
	}


	return true;
}

// Controls what the player does when it collides with another collider
void Player::OnCollision(Collider* c1, Collider* c2)
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
		app->audio->PlayFX(nextLevelFX, 0);
		isWinning = true;
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::DIE)
	{
		isDying = true;
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::CHECKPOINT)
	{
		app->audio->PlayFX(checkpointFX, 0);
		if (app->currentScene == LEVEL1)
		{
			app->level1->Cp1Activation();
		}
		if (app->currentScene == LEVEL2)
		{
			app->level2->Cp2Activation();
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
		app->potion->isCollected = true;
	}
}

bool Player::LoadState(pugi::xml_node& playerNode)
{
	// Load current scene
	sc = playerNode.child("level").attribute("currentlevel").as_int();
	switch (app->player->sc)
	{
	case 1:
		if (app->currentScene == LEVEL2)
		{
			app->fadeToBlack->Fade((Module*)app->level2, (Module*)app->level1, 60.0f);
		}
		app->currentScene = LEVEL1;
		break;

	case 2:
		if (app->currentScene == LEVEL1)
		{
			app->fadeToBlack->Fade((Module*)app->level1, (Module*)app->level2, 60.0f);
		}
		app->currentScene = LEVEL2;
		break;
	}

	// Load position
	position.x = playerNode.child("position").attribute("positionX").as_float();
	position.y = playerNode.child("position").attribute("positionY").as_float();

	// Load velocity
	velocity.x = playerNode.child("velocity").attribute("velocityX").as_float();
	velocity.y = playerNode.child("velocity").attribute("velocityY").as_float();

	// Load state
	st = playerNode.child("state").attribute("playerstate").as_int();
	switch (st)
	{
	case PlayerState::IDLE:
		if (state = GOD_MODE)
		{
			playerCollider->type = Collider::ColliderType::PLAYER;
			godMode = false;
		}
		state = IDLE;
		currentAnimation->Reset();
		currentAnimation = idleAnim;
		break;

	case PlayerState::MOVE_RIGHT:
		if (state = GOD_MODE)
		{
			playerCollider->type = Collider::ColliderType::PLAYER;
			godMode = false;
		}
		state = MOVE_RIGHT;
		currentAnimation->Reset();
		currentAnimation = walkAnim;
		break;

	case PlayerState::MOVE_LEFT:
		if (state = GOD_MODE)
		{
			playerCollider->type = Collider::ColliderType::PLAYER;
			godMode = false;
		}
		state = MOVE_LEFT;
		currentAnimation->Reset();
		currentAnimation = walkAnim;
		break;

	case PlayerState::JUMP:
		if (state = GOD_MODE)
		{
			playerCollider->type = Collider::ColliderType::PLAYER;
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
	horizontalDirection = playerNode.child("state").attribute("horizontaldirection").as_int();

	return true;
}


bool Player::SaveState(pugi::xml_node& playerNode) const
{
	pugi::xml_node player = playerNode.append_child("position");
	player.append_attribute("positionX").set_value(position.x);
	player.append_attribute("positionY").set_value(position.y);

	player = playerNode.append_child("velocity");
	player.append_attribute("velocityX").set_value(velocity.x);
	player.append_attribute("velocityY").set_value(velocity.y);

	player = playerNode.append_child("state");
	switch (state)
	{
	case IDLE:
		app->player->st = 0;
		break;

	case MOVE_RIGHT:
		app->player->st = 1;
		break;

	case MOVE_LEFT:
		app->player->st = 2;
		break;

	case JUMP:
		app->player->st = 3;
		break;

	case GOD_MODE:
		app->player->st = 4;
		break;
	}
	player.append_attribute("playerstate").set_value(st);
	player.append_attribute("horizontaldirection").set_value(horizontalDirection);

	player = playerNode.append_child("level");
	switch (app->currentScene)
	{
	case LEVEL1:
		app->player->sc = 1;
		break;

	case LEVEL2:
		app->player->sc = 2;
		break;
	}
	player.append_attribute("currentlevel").set_value(sc);


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
		timeInAir = 0.0f;
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
		velocity.y = vel.y;
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
	
	
	timeInAir = timeInAir + 0.05f;

	
	acceleration.y = accel.y;
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
		position.y = c->rect.y - PLAYER_SIZE;
		isTouchingGround = true;
	}
}


void Player::ControlCameraMovement(Collider* c)
{
	if (position.x < c->rect.x)
	{
		c->rect.x -= c->rect.x - position.x;
	}
	if ((position.x + PLAYER_SIZE) > (c->rect.x + c->rect.w))
	{
		c->rect.x += (position.x + PLAYER_SIZE) - (c->rect.x + c->rect.w);
	}
	if (position.y < c->rect.y)
	{
		c->rect.y -= c->rect.y - position.y;
	}
	if ((position.y + PLAYER_SIZE) > (c->rect.y + c->rect.h))
	{
		c->rect.y += (position.y + PLAYER_SIZE) - (c->rect.y + c->rect.h);
	}

	cameraCollider->rect = c->rect;

}


// Clean Up
bool Player::CleanUp()
{
	if (!active)
	{
		return true;
	}

	app->tex->UnLoad(playerTexture);
	app->tex->UnLoad(lifesTexture);

	app->audio->UnloadFX(jumpFX);
	app->audio->UnloadFX(secondJumpFX);
	app->audio->UnloadFX(oofFX);
	app->audio->UnloadFX(checkpointFX);
	app->audio->UnloadFX(nextLevelFX);

	app->collisions->RemoveCollider(playerCollider);
	app->collisions->RemoveCollider(cameraCollider);

	active = false;

	return true;
}