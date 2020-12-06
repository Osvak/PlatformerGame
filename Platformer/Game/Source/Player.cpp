#include "Player.h"

#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Scene.h"

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
	playerTexture = app->tex->Load("Assets/textures/character/adventurer-v1.5-Sheet.png");

	//
	//
	//
	// Load Player FX files
	//
	//
	//


	//
	// Set initial position
	//
	position.x = 48.0f; //Tile size * Tiles
	position.y = 176.0f; //Tile size * Tiles


	//
	//
	// Create Player collider
	//
	playerCollider = app->collisions->AddCollider({ (int)position.x , (int)position.y, PLAYER_SIZE, PLAYER_SIZE }, Collider::ColliderType::PLAYER, this);

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
	isTouchingGround = true;
	isJumping = false;
	isWinning = false;
	isDying = false;
	lifes = 3;
	savedPos = { 48.0f, 176.0f };
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

		if (isTouchingGround ==  true && app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
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
	// Next Level control
	//
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

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::PLATFORM)
	{
		ControlPlatformCollision(c2);
		rect = c2->rect;
	}
	else
	{
		isTouchingGround = false;
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::NEXT_LEVEL)
	{
		isWinning = true;
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::DIE)
	{
			isDying = true;
	}

	if (c1->type == Collider::ColliderType::PLAYER && c2->type == Collider::ColliderType::CHECKPOINT)
	{
		app->scene->Cp1Activation();
	}
}

bool Player::LoadState(pugi::xml_node& playerNode)
{
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

	// Load current scene
	switch (app->player->sc)
	{
	case 1:
		if (app->currentScene == LEVEL2)
		{
			app->fadeToBlack->Fade((Module*)app->scene/*2*/, (Module*)app->scene, 60.0f);
		}
		app->currentScene = LEVEL1;
		break;

	case 2:
		app->currentScene = LEVEL2;
		break;
	}



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



// Clean Up
bool Player::CleanUp()
{
	if (!active)
	{
		return true;
	}

	app->tex->UnLoad(playerTexture);

	active = false;

	return true;
}