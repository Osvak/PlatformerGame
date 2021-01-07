#include "EnemyGhost.h"

#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "Player.h"
#include "Map.h"
#include "Pathfinding.h"

#include "Log.h"
#include "Defs.h"



// Constructor
EnemyGhost::EnemyGhost(Render* render, Textures* tex, AudioManager* audioManager, PathFinding* pathFinding) : Entity(EntityType::ENEMY_GHOST)
{
	LOG("Creating Ghost Entity");

	name.Create("ghost");


	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->pathFinding = pathFinding;


	//
	// Animation pushbacks
	//
	idleAnim->loop = true;
	idleAnim->speed = 0.1f;
	for (int i = 0; i < 7; ++i)
	{
		idleAnim->PushBack({ 47 * i, 0, 47, 45 });
	}
	aggroAnim->loop = false;
	aggroAnim->speed = 0.1f;
	for (int i = 0; i < 4; ++i)
	{
		aggroAnim->PushBack({ 47 * i, 45, 47, 45 });
	}
	attackAnim->loop = true;
	attackAnim->speed = 0.1f;
	for (int i = 0; i < 2; ++i)
	{
		attackAnim->PushBack({ 47 * i, 90, 47, 45 });
	}
	deaggroAnim->loop = false;
	deaggroAnim->speed = 0.1f;
	for (int i = 0; i < 4; ++i)
	{
		deaggroAnim->PushBack({ 47 * i, 135, 47, 45 });
	}
	deathAnim->loop = false;
	deathAnim->speed = 0.2f;
	for (int i = 0; i < 6; ++i)
	{
		deathAnim->PushBack({ 47 * i, 180, 47, 45 });
	}
	

	//
	// Load Ghost texture files
	//
	ghostTexture = tex->Load("Assets/Textures/Enemies/ghost_spritesheet.png");

	//
	// Set current animation
	//
	currentAnimation = idleAnim;

	//
	// Set current state
	//
	state = GHOST_IDLE;

	//
	// Set Flags and Variables
	//
	width = GHOST_WIDTH;
	height = GHOST_HEIGHT;
}
// Destructor
EnemyGhost::~EnemyGhost()
{

}


// Checks the intersection between two rectangles
inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}
// Ghost Update called every loop
bool EnemyGhost::Update(float dt, Player* player, Map* map)
{
	if (isDestroyed == false)
	{
		this->map = map;
		this->player = player;

		UpdateState();
		UpdateLogic(dt);
	}

	return true;
}
// Control input and states
void EnemyGhost::UpdateState()
{
	switch (state)
	{
	case GHOST_IDLE:
	{
		if (Radar(player->position, visionRange) == true)
		{
			ChangeState(state, GHOST_AGGRO);
		}

		// Check for ghost death
		if (isHit == true)
		{
			ChangeState(state, GHOST_DYING);
		}


		break;
	}

	case GHOST_AGGRO:
	{
		if (aggroFinished == true)
		{
			if (Radar(player->position, visionRange) == true)
			{
				ChangeState(state, GHOST_ATTACK);
			}
			else
			{
				ChangeState(state, GHOST_DEAGGRO);
			}
		}

		// Check for ghost death
		if (isHit == true)
		{
			ChangeState(state, GHOST_DYING);
		}


		break;
	}

	case GHOST_ATTACK:
	{
		if (Radar(player->position, visionRange) == false)
		{
			ChangeState(state, GHOST_DEAGGRO);
		}

		// Check for ghost death
		if (isHit == true)
		{
			ChangeState(state, GHOST_DYING);
		}


		break;
	}

	case GHOST_DEAGGRO:
	{
		if (deaggroFinished == true)
		{
			if (Radar(player->position, visionRange) == true)
			{
				ChangeState(state, GHOST_AGGRO);
			}
			else
			{
				ChangeState(state, GHOST_IDLE);
			}
		}

		// Check for ghost death
		if (isHit == true)
		{
			ChangeState(state, GHOST_DYING);
		}


		break;
	}

	case GHOST_DYING:
	{
		// Nothing to do here


		break;
	}

	default:
		break;
	}
}
// Controls what each state does
void EnemyGhost::UpdateLogic(float dt)
{
	switch (state)
	{
	case GHOST_IDLE:
	{
		// Make sure the ghost doesn't move when it's idle
		velocity = { 0,0 };

		// Update idle Animation
		currentAnimation->Update();


		break;
	}

	case GHOST_AGGRO:
	{
		// Make sure the ghost doesn't move when it's aggroing
		velocity = { 0,0 };

		// Update aggro Animation
		currentAnimation->Update();

		// If the aggro animation has finished, leave the aggro state
		if (currentAnimation->HasFinished())
		{
			aggroFinished = true;
		}


		break;
	}

	case GHOST_ATTACK:
	{
		// Convert world position to map position
		iPoint ghostTile = map->WorldToMap((int)position.x, (int)position.y + (GHOST_HEIGHT - TILE_SIZE)); // Ghost's position
		iPoint playerTile = map->WorldToMap((int)player->position.x, (int)player->position.y + (28 - TILE_SIZE)); // Player's position

		// Create new path
		pathCreated = pathFinding->CreatePath(ghostTile, playerTile);
		path = pathFinding->GetLastPath();

		if (path->At(0) != nullptr)
		{
			// TODO: needs to check path position instead of player position
			// Change facing direction
			if (player->position.x + player->playerWidth < position.x)
			{
				horizontalDirection = -1;
			}
			if (player->position.x > position.x + width)
			{
				horizontalDirection = 1;
			}
			if (player->position.y > position.y + height)
			{
				verticalDirection = 1;
			}
			else if (player->position.y + player->playerHeight < position.y)
			{
				verticalDirection = -1;
			}
			else
			{
				verticalDirection = 0;
			}
			velocity.x = horizontalDirection * GHOST_SPEED;
			velocity.y = verticalDirection * GHOST_SPEED;
		}

		// Update attack Animation
		currentAnimation->Update();


		break;
	}

	case GHOST_DEAGGRO:
	{
		// Make sure the ghost doesn't move when it's deaggroing
		velocity = { 0,0 };

		// Update deaggro Animation
		currentAnimation->Update();

		// If the deaggro animation has finished, leave the deaggro state
		if (currentAnimation->HasFinished())
		{
			deaggroFinished = true;
		}


		break;
	}

	case GHOST_DYING:
	{
		// Make sure the ghost doesn't move when it's dying
		velocity = { 0,0 };

		// Update death Animation
		currentAnimation->Update();

		// If the death Animation has finished, destroy the ghost
		if (currentAnimation->HasFinished() == true)
		{
			isDestroyed = true;
		}


		break;
	}

	default:
		break;
	}

	// Update position
	velocity.x = velocity.x * dt;
	velocity.y = velocity.y * dt;
	position.x += velocity.x;
	position.y += velocity.y;

}
// Changes the state
void EnemyGhost::ChangeState(GhostState previousState, GhostState newState)
{
	switch (newState)
	{
	case GHOST_IDLE:
	{
		currentAnimation = idleAnim;


		break;
	}

	case GHOST_AGGRO:
	{
		currentAnimation = aggroAnim;
		currentAnimation->Reset();
		aggroFinished = false;


		break;
	}

	case GHOST_ATTACK:
	{
		currentAnimation = attackAnim;
		currentAnimation->Reset();

		if (player->position.x > position.x + width)
		{
			horizontalDirection = 1;
		}
		else if (player->position.x + player->playerWidth < position.x)
		{
			horizontalDirection = -1;
		}


		break;
	}

	case GHOST_DEAGGRO:
	{
		currentAnimation = deaggroAnim;
		currentAnimation->Reset();
		deaggroFinished = false;


		break;
	}

	case GHOST_DYING:
	{
		currentAnimation = deathAnim;
		currentAnimation->Reset();

		break;
	}

	default:
		break;
	}


	state = newState;
}


// Draws the ghost
bool EnemyGhost::Draw()
{
	//
	// Draw Ghost
	//
	if (isDestroyed == false)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();

		if (horizontalDirection == 1)
		{
			render->DrawFlippedTexture(ghostTexture, (int)position.x - 14, (int)position.y - 4, &rect);
		}
		if (horizontalDirection == -1)
		{
			render->DrawTexture(ghostTexture, (int)position.x - 14, (int)position.y - 4, &rect);
		}
	}


	return true;
}
void EnemyGhost::DrawColliders()
{
	if (isDestroyed == false)
	{
		render->DrawRectangle(GetRect(), 255, 0, 0, 120);

		if (pathCreated != -1)
		{
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint nextPoint = map->MapToWorld(path->At(i)->x, path->At(i)->y);
				SDL_Rect pathRect = { nextPoint.x, nextPoint.y, TILE_SIZE, TILE_SIZE };
				render->DrawRectangle(pathRect, 255, 0, 0, 50);
			}
		}
	}
}


// Release memory
bool EnemyGhost::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Unloading Ghost");

	tex->UnLoad(ghostTexture);

	active = false;

	return true;
}


// Loads the ghost state
bool EnemyGhost::LoadState(pugi::xml_node& ghostNode)
{
	// Load position
	position.x = ghostNode.child("position").attribute("position_x").as_float();
	position.y = ghostNode.child("position").attribute("position_y").as_float();

	// Load velocity
	velocity.x = ghostNode.child("velocity").attribute("velocity_x").as_float();
	velocity.y = ghostNode.child("velocity").attribute("velocity_y").as_float();

	// Load state info
	st = ghostNode.child("state").attribute("ghost_state").as_int();
	switch (st)
	{
	case GhostState::GHOST_IDLE:
		state = GHOST_IDLE;
		currentAnimation = idleAnim;
		break;
	case GhostState::GHOST_AGGRO:
		state = GHOST_AGGRO;
		currentAnimation = aggroAnim;
		break;
	case GhostState::GHOST_ATTACK:
		state = GHOST_ATTACK;
		currentAnimation = attackAnim;
		break;
	case GhostState::GHOST_DEAGGRO:
		state = GHOST_DEAGGRO;
		currentAnimation = deaggroAnim;
		break;
	case GhostState::GHOST_DYING:
		state = GHOST_DYING;
		currentAnimation = deathAnim;
		break;
	}
	currentAnimation->SetCurrentFrame(ghostNode.child("state").attribute("current_state").as_int());
	horizontalDirection = ghostNode.child("state").attribute("horizontal_direction").as_int();


	return true;
}
// Saves the ghost state
bool EnemyGhost::SaveState(pugi::xml_node& ghostNode) const
{
	// Save position
	pugi::xml_node ghost = ghostNode.append_child("position");
	ghost.append_attribute("position_x").set_value(position.x);
	ghost.append_attribute("position_y").set_value(position.y);

	// Save velocity
	ghost = ghostNode.append_child("velocity");
	ghost.append_attribute("velocity_x").set_value(velocity.x);
	ghost.append_attribute("velocity_y").set_value(velocity.y);

	// Save state info
	ghost = ghostNode.append_child("state");
	switch (state)
	{
	case GHOST_IDLE:
		st = 0;
		break;
	case GHOST_AGGRO:
		st = 1;
		break;
	case GHOST_ATTACK:
		st = 2;
		break;
	case GHOST_DEAGGRO:
		st = 3;
		break;
	case GHOST_DYING:
		st = 4;
		break;
	}
	ghost.append_attribute("ghost_state").set_value(st);
	ghost.append_attribute("horizontal_direction").set_value(horizontalDirection);


	return true;
}


// Getter of the ghosts's rectangle
SDL_Rect EnemyGhost::GetRect()
{
	return  { (int)position.x, (int)position.y, width, height };
}


// Check if the player is in range of the Ghost
bool EnemyGhost::Radar(fPoint playerPosition, int range)
{
	if (position.DistanceManhattan(playerPosition) < range)
	{
		return true;
	}
	else
	{
		return false;
	}
}