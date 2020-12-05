#include "Player.h"

#include "App.h"

#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Collisions.h"
#include "Window.h"

#include "Render.h"


#include "SDL/include/SDL_scancode.h"
#include <stdio.h>



Player::Player() : Module()
{
	name.Create("player");
}

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
	walkAnim->speed = 0.4f;
	jumpAnim->loop = false;
	jumpAnim->speed = 0.25f;
	fallAnim->loop = true;
	fallAnim->speed = 0.2f;


	for (int i = 0; i < 4; i++)
		idleAnim->PushBack({ 50 * i,0, 50, 37 });

	for (int i = 0; i < 6; i++)
		walkAnim->PushBack({ 50 + (50 * i),37, 50, 37 });

	for (int i = 0; i < 7; i++)
		jumpAnim->PushBack({ 50 * i, 74, 50, 37 });
		jumpAnim->PushBack({ 0, 111, 50, 37 });

	for (int i = 0; i < 2 ; i++)
		fallAnim->PushBack({ 50 + (50 * i) , 111, 50, 37 });

	currentAnimation = idleAnim;


	return ret;
}

// Called the first frame
bool Player::Start()
{
	LOG("+++++ Loading player textures");

	destroyed = false;
	movingFlag = false;



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
	// Set Flags and Variables
	//
	active = true;
	velocity = { 0.0f, 0.0f };
	acceleration = { 0.0f, 0.0f };
	


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
		


		ChangeState(state, IDLE);

		break;
	}

	case JUMP:
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

			ChangeState(state, IDLE);
		}
		break;
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
		currentAnimation->Update();
		break;
	}

	case MOVE_RIGHT:
	{
		currentAnimation->Update();
		break;
	}

	case MOVE_LEFT:
	{

		currentAnimation->Update();
		break;
	}

	case JUMP:
		if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN))
		{
			velocity.x = PLAYER_SPEED;
			horizontalDirection = 1;
		}
		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN))
		{
			velocity.x = -PLAYER_SPEED;
			horizontalDirection = -1;
		}
		Jump(dt);
		currentAnimation->Update();
		break;

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

	position.x += velocity.x;
	position.y += velocity.y;


	//
	// Update Collider Position
	//
	playerCollider->SetPos(position.x, position.y);

}

// Change the State
void Player::ChangeState(PlayerState previousState, PlayerState newState)
{
	switch (newState)
	{
	case IDLE:
	{
		verticalDirection = 0;
		currentAnimation = idleAnim;

		velocity = { 0, 0 };
		acceleration = { 0,0 };
		break;
	}

	case MOVE_RIGHT:
	{
		currentAnimation = walkAnim;
		verticalDirection = 0;
		horizontalDirection = 1;

		velocity.x = PLAYER_SPEED;

		break;
	}

	case MOVE_LEFT:
	{
		currentAnimation = walkAnim;
		verticalDirection = 0;
		horizontalDirection = -1;

		velocity.x = -PLAYER_SPEED;

		break;
	}

	case JUMP:
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
		verticalDirection = 0;
		isTouchingGround = true;
		isJumping = false;
		position.y = c2->rect.y - (PLAYER_SIZE);
	}
	else
	{
		isTouchingGround = false;
	}
}

bool Player::LoadState(pugi::xml_node& playerNode)
{
	position.x = playerNode.child("position").attribute("positionX").as_float();
	position.y = playerNode.child("position").attribute("positionY").as_float();

	return true;
}


bool Player::SaveState(pugi::xml_node& playerNode) const
{
	pugi::xml_node player = playerNode.append_child("position");
	player.append_attribute("positionX").set_value(position.x);
	player.append_attribute("positionY").set_value(position.y);
	return true;
}


void Player::Jump(float dt)
{
	// Allow player to jump
	if (isTouchingGround) {
		timeInAir = 0.0f;
		accel.y = 0.0;
		vel.y = 0.0;
		isTouchingGround = false;
	}


	if (timeInAir < jumpImpulseTime)
	{
		vel.y = jumpImpulseVel;
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
	}
	
	
	timeInAir = timeInAir + 0.05f;

	
	acceleration.y = accel.y;
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