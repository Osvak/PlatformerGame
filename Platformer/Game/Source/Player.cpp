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
	//
	//
	// Animation pushbacks
	//
	//
	//

	//
	// Set initial position
	//
	position.x = 48.0f; //Tile size * Tiles
	position.y = 176.0f; //Tile size * Tiles

	//
	// Placeholder square
	//
	plSquare = { (int)position.x, (int)position.y, PLAYER_SIZE, PLAYER_SIZE };



	//
	// Set Flags and Variables
	//
	velocity = { 0.0f, 0.0f };
	acceleration = { 0.0f, 0.0f };

	return ret;
}

// Called the first frame
bool Player::Start()
{
	LOG("+++++ Loading player textures");

	destroyed = false;
	movingFlag = false;


	//
	//
	//
	// Load Player textures files & set currentAnimation
	//
	//
	//

	//
	//
	//
	// Load Player FX files
	//
	//
	//


	//
	//
	// Create Player collider
	//
	playerCollider = app->collisions->AddCollider({ (int)position.x , (int)position.y, PLAYER_SIZE, PLAYER_SIZE }, Collider::ColliderType::PLAYER, this);



	//
	// Placeholder square
	//
	plSquare = { (int)position.x, (int)position.y, PLAYER_SIZE, PLAYER_SIZE };

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
		// Nothing to do here :)
		break;
	}

	case MOVE_RIGHT:
	{
		//currentAnimation->Update();
		break;
	}

	case MOVE_LEFT:
	{

		//currentAnimation->Update();
		break;
	}

	case JUMP:
		Jump(dt);

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

	//
	// Update placeholder square position
	//
	plSquare.x = position.x;
	plSquare.y = position.y;
}

// Change the State
void Player::ChangeState(PlayerState previousState, PlayerState newState)
{
	switch (newState)
	{
	case IDLE:
	{
		verticalDirection = 0;
		horizontalDirection = 0;
		//currentAnimation = &(horizontalDirection == -1 ? idleAnim_Left : idleAnim_Right);

		velocity = { 0, 0 };
		acceleration = { 0,0 };
		break;
	}

	case MOVE_RIGHT:
	{
		verticalDirection = 0;
		horizontalDirection = 1;

		velocity.x = PLAYER_SPEED;

		break;
	}

	case MOVE_LEFT:
	{
		verticalDirection = 0;
		horizontalDirection = -1;

		velocity.x = -PLAYER_SPEED;

		break;
	}

	case JUMP:
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
		else
		{
			horizontalDirection = 0;
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
		/*SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(playerTexture, position.x, position.y, &rect);*/
		app->render->DrawRectangle(plSquare, 0, 255, 0, 255);
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
	
	
	timeInAir = timeInAir + 0.05f;

	
	acceleration.y = accel.y;
}