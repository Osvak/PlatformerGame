#include "Player.h"

#include "App.h"

#include "Textures.h"
#include "Input.h"
#include "Map.h"
//#include "Collisions.h"

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

	LOG("+++++ Loading player textures");

	destroyed = false;
	movingFlag = false;


	//
	// Set initial position
	//
	int screenWidth = config.child("resolution").attribute("width").as_int();
	int screenHeight = config.child("resolution").attribute("height").as_int();
	position.x = 48.0f; //Tile size * Tiles
	position.y = 176.0f; //Tile size * Tiles


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
	//
	// Create Player collider
	//
	//
	//


	//
	// Placeholder square
	//
	plSquare = { (int)position.x, (int)position.y, PLAYER_SIZE, PLAYER_SIZE };



	return ret;
}

// Main player Update
bool Player::Update(float dt)
{
	UpdateState();
	UpdateLogic();

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

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(state, JUMP);
			break;
		}


		ChangeState(state, IDLE);

		break;
	}

	case MOVE_LEFT:
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

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(state, JUMP);
			break;
		}


		ChangeState(state, IDLE);

		break;
	}
	default:
		break;
	}
}

// Control what each state does
void Player::UpdateLogic()
{
	switch (state)
	{
	case IDLE:
	{
		// Nothing to do here :)
		break;
	}

	case MOVE_RIGHT:
	{
		position.x += speed * horizontalDirection;

		//currentAnimation->Update();
		break;
	}

	case MOVE_LEFT:
	{
		position.x += speed * horizontalDirection;

		//currentAnimation->Update();
		break;
	}

	case JUMP:
		break;

	default:
		break;
	}


	//
	// Update Collider Position
	//
	//collider->SetPos(position.x + 2, position.y + 14);



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
		//currentAnimation = &(horizontalDirection == -1 ? idleAnim_Left : idleAnim_Right);
		break;
	}

	case MOVE_RIGHT:
	{
		verticalDirection = 0;
		horizontalDirection = 1;

		break;
	}

	case(MOVE_LEFT):
	{
		verticalDirection = 0;
		horizontalDirection = -1;

		break;
	}
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
//void Player::OnCollision(Collider* c1, Collider* c2)
//{
	//
	//
	//
	// Collision control
	//
	//
	//

	/*if (c1->type == Collider::Type::PLAYER && c2->type == Collider::Type::LADDER)
	{
		canClimb = true;
	}

	if (c2->type == Collider::Type::TOP_LADDER)
	{
		// TODO 5: The player has reached the top of the ladder, stop climbing


	}*/
//}