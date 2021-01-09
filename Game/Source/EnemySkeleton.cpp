#include "EnemySkeleton.h"

#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "Player.h"
#include "Map.h"
#include "Pathfinding.h"

#include "Log.h"
#include "Defs.h"



// Constructor
EnemySkeleton::EnemySkeleton(Render* render, Textures* tex, AudioManager* audioManager, PathFinding* pathFinding) : Entity(EntityType::ENEMY_SKELETON)
{
	LOG("Creating Skeleton Entity");

	name.Create("skeleton");


	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;
	this->pathFinding = pathFinding;


	//
	// Animation pushbacks
	//
	attackAnim->loop = false;
	attackAnim->speed = 0.2f;
	for (int i = 0; i < 18; ++i)
	{
		attackAnim->PushBack({ 43 * i, 0, 43, 37 });
	}
	idleAnim->loop = true;
	idleAnim->pingPong = true;
	idleAnim->speed = 0.1f;
	for (int i = 0; i < 11; ++i)
	{
		idleAnim->PushBack({ 43 * i, 37, 43, 37 });
	}
	walkAnim->loop = true;
	walkAnim->speed = 0.2f;
	for (int i = 0; i < 13; ++i)
	{
		walkAnim->PushBack({ 43 * i, 74, 43, 37 });
	}
	deathAnim->loop = false;
	deathAnim->speed = 0.2f;
	for (int i = 0; i < 15; ++i)
	{
		deathAnim->PushBack({ 43 * i, 111, 43, 37 });
	}

	//
	// Load Skeleton texture files
	//
	skeletonTexture = tex->Load("Assets/Textures/Enemies/skeleton_spritesheet.png");

	//
	// Load Skeleton FX files
	//
	attackFX = audioManager->LoadFX("Assets/Audio/FX/skeleton_attack.wav");
	audioManager->musicList.Add(&attackFX);
	deathFX = audioManager->LoadFX("Assets/Audio/FX/skeleton_death.wav");
	audioManager->musicList.Add(&deathFX);

	//
	// Set current animation
	//
	currentAnimation = idleAnim;
	
	//
	// Set current state
	//
	state = SKELETON_IDLE;

	//
	// Set Flags and Variables
	//
	width = SKELETON_WIDTH;
	height = SKELETON_HEIGHT;
	playFX = true;
}
// Destructor
EnemySkeleton::~EnemySkeleton()
{

}


// Checks the intersection between two rectangles
inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}
// Skeleton Update called every loop
bool EnemySkeleton::Update(float dt, Player* player, Map* map)
{
	this->map = map;
	this->player = player;

	if (isDestroyed == false)
	{
		UpdateState();
		UpdateLogic(dt);
	}

	return true;
}
// Control input and states
void EnemySkeleton::UpdateState()
{
	switch (state)
	{
	case SKELETON_IDLE:
	{
		if (Radar(player->position, visionRange) == true)
		{
			ChangeState(state, SKELETON_MOVE);
			break;
		}
		if (horizontalDirection == -1)
		{
			if (Radar(player->position, attackRange + 4) == true)
			{
				ChangeState(state, SKELETON_ATTACK);
				break;
			}
		}
		else
		{
			if (Radar(player->position, attackRange) == true)
			{
				ChangeState(state, SKELETON_ATTACK);
				break;
			}
		}

		// Check for skeleton death
		if (isHit == true)
		{
			ChangeState(state, SKELETON_DYING);
			break;
		}


		break;
	}

	case SKELETON_MOVE:
	{
		if (Radar(player->position, visionRange) == false)
		{
			ChangeState(state, SKELETON_IDLE);
			break;
		}
		if (horizontalDirection == -1)
		{
			if (Radar(player->position, attackRange + 4) == true)
			{
				ChangeState(state, SKELETON_ATTACK);
				break;
			}
		}
		else
		{
			if (Radar(player->position, attackRange) == true)
			{
				ChangeState(state, SKELETON_ATTACK);
				break;
			}
		}

		// Check for skeleton death
		if (isHit == true)
		{
			ChangeState(state, SKELETON_DYING);
			break;
		}


		break;
	}

	case SKELETON_ATTACK:
	{
		if ((Radar(player->position, attackRange) == false) && (attackFinished == true))
		{
				ChangeState(state, SKELETON_IDLE);
				break;
		}

		// Check for skeleton death
		if (isHit == true)
		{
			ChangeState(state, SKELETON_DYING);
			break;
		}


		break;
	}

	case SKELETON_DYING:
	{
		// Nothing to do here

		break;
	}

	default:
		break;
	}
}
// Controls what each state does
void EnemySkeleton::UpdateLogic(float dt)
{
	switch (state)
	{
	case SKELETON_IDLE:
	{
		// Make sure the skeleton doesn't move when it's idle
		velocity = { 0.0f,0.0f };

		// Update idle Animation
		currentAnimation->Update();


		break;
	}

	case SKELETON_MOVE:
	{
		// Convert world position to map position
		iPoint skeletonTile = map->WorldToMap((int)position.x, (int)position.y + (SKELETON_HEIGHT - TILE_SIZE)); // Skeleton's position
		iPoint playerTile = map->WorldToMap((int)player->position.x, (int)player->position.y + (28 - TILE_SIZE)); // Player's position
		
		// Create new path
		pathCreated = pathFinding->CreatePath(skeletonTile, playerTile);
		path = pathFinding->GetLastPath();

		if (path->At(0) != nullptr)
		{
			iPoint skTilePerfect = map->MapToWorld(skeletonTile.x, skeletonTile.y); // Limits of the current tile

			if (skTilePerfect.x == (int)position.x || skTilePerfect.x + TILE_SIZE == (int)position.x + width) // Checks to see if the skeleton has finished moving in this tile
			{
				MapLayer* layer;
				for (ListItem<MapLayer*>* item = map->data.layers.start; item; item = item->next)
				{
					layer = item->data;
					if (layer->properties.GetProperty("navigation", 0) == 0) // looks at navigation layer
					{
						continue;
					}
					int tileId;
					if (position.x > player->position.x) // Player is left of the skeleton
					{
						tileId = layer->Get(skeletonTile.x - 1, skeletonTile.y + 1); // Checks if the next walkable tile is actually walkable
						if (tileId == 5)
						{
							canWalk = true;
						}
						else
						{
							canWalk = false;
						}
					}
					if (position.x + width < player->position.x) // Player is right of the skeleton
					{
						tileId = layer->Get(skeletonTile.x + 1, skeletonTile.y + 1); // Checks if the next walkable tile is actually walkable
						if (tileId == 5)
						{
							canWalk = true;
						}
						else
						{
							canWalk = false;
						}
					}
				}
			}
			else
			{
				// Change facing direction
				if (player->position.x + player->width < position.x)
				{
					horizontalDirection = -1;
				}
				if (player->position.x > position.x + width)
				{
					horizontalDirection = 1;
				}
			}
		}

		// Control for when the skeleton can or can't walk
		if (canWalk == true)
		{
			currentAnimation = walkAnim;
			velocity.x = horizontalDirection * SKELETON_SPEED;
		}
		else
		{
			currentAnimation = idleAnim;
		}

		// Update walk/idle Animation
		currentAnimation->Update();


		break;
	}

	case SKELETON_ATTACK:
	{
		if (currentAnimation->HasFinished() == true) // Checks if an attack has finished
		{
			playFX = true;
			attackFinished = true;
			currentAnimation->Reset();
		}

		if (attackFinished == true) // Attack is on cooldown after an attack
		{
			++attackCooldwon;

			if (attackCooldwon == ATTACK_COOLDOWN)
			{
				attackFinished = false;
				attackCooldwon = 0;
			}

			// During this cooldown, the skeleton can change looking direction
			if (player->position.x > position.x + width)
			{
				horizontalDirection = 1;
			}
			else if (player->position.x + 12 < position.x)
			{
				horizontalDirection = -1;
			}
		}
		else // Things the skeleton does DURING an attack
		{
			if (playFX == true)
			{
				audioManager->PlayFX(attackFX);
				playFX = false;
			}

			currentAnimation->Update();

			// The Skeleton can HIT during these frames of the animation
			switch (currentAnimation->GetCurrentFrameID())
			{
			case 7:
			{
				if (horizontalDirection == -1)
				{
					attackRect = { (int)position.x - 27, (int)position. y, 11, 22 };
				}
				else
				{
					attackRect = { (int)position.x + 25 , (int)position.y, 11, 22 };
				}

				// Checks the collision between the axe hit and the player for this frame
				player->isHit = CheckCollision(attackRect, player->GetRect());


				break;
			}
			case 8:
			{
				if (horizontalDirection == -1)
				{
					attackRect = { (int)position.x - 27 , (int)position.y + 11, 11, 11 };
				}
				else
				{
					attackRect = { (int)position.x + 25 , (int)position.y + 11, 11, 11 };
				}

				// Checks the collision between the axe hit and the player for this frame
				player->isHit = CheckCollision(attackRect, player->GetRect());


				break;
			}

			default:
			{
				attackRect = { 0,0,0,0 };

				break;
			}
			}

		}


		break;
	}

	case SKELETON_DYING:
	{
		if (playFX == true)
		{
			audioManager->PlayFX(deathFX);
			playFX = false;
		}
		currentAnimation->Update();

		if (currentAnimation->HasFinished() == true)
		{
			playFX = true;
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
void EnemySkeleton::ChangeState(SkeletonState previousState, SkeletonState newState)
{
	switch (newState)
	{
	case SKELETON_IDLE:
	{
		currentAnimation = idleAnim;

		attackFinished = false;


		break;
	}

	case SKELETON_MOVE:
	{
		currentAnimation = walkAnim;

		break;
	}

	case SKELETON_ATTACK:
	{
		currentAnimation = attackAnim;
		currentAnimation->Reset();

		if (player->position.x > position.x + width)
		{
			horizontalDirection = 1;
		}
		else if (player->position.x + 12 < position.x)
		{
			horizontalDirection = -1;
		}


		break;
	}

	case SKELETON_DYING:
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


// Draws the skeleton
bool EnemySkeleton::Draw()
{
	//
	// Draw Skeleton
	//
	if (!isDestroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();

		if (horizontalDirection == 1)
		{
			render->DrawTexture(skeletonTexture, (int)position.x - 5, (int)position.y - 15, &rect);
		}
		if (horizontalDirection == -1)
		{
			render->DrawFlippedTexture(skeletonTexture, (int)position.x - 28, (int)position.y - 15, &rect);
		}
	}


	return true;
}
void EnemySkeleton::DrawColliders()
{
	render->DrawRectangle(GetRect(), 255, 0, 0, 120);

	render->DrawRectangle(attackRect, 0, 0, 255, 100);

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


// Release memory
bool EnemySkeleton::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Unloading Skeleton");

	tex->UnLoad(skeletonTexture);

	active = false;

	return true;
}


// Loads the skeleton state
bool EnemySkeleton::LoadState(pugi::xml_node& skeletonNode)
{
	// Load position
	position.x = skeletonNode.child("position").attribute("position_x").as_float();
	position.y = skeletonNode.child("position").attribute("position_y").as_float();

	// Load velocity
	velocity.x = skeletonNode.child("velocity").attribute("velocity_x").as_float();
	velocity.y = skeletonNode.child("velocity").attribute("velocity_y").as_float();

	// Load state info
	st = skeletonNode.child("state").attribute("skeleton_state").as_int();
	switch (st)
	{
	case SkeletonState::SKELETON_IDLE:
		state = SKELETON_IDLE;
		currentAnimation = idleAnim;
		break;
	case SkeletonState::SKELETON_MOVE:
		state = SKELETON_MOVE;
		currentAnimation = walkAnim;
		break;
	case SkeletonState::SKELETON_ATTACK:
		state = SKELETON_ATTACK;
		currentAnimation = attackAnim;
		break;
	case SkeletonState::SKELETON_DYING:
		state = SKELETON_DYING;
		currentAnimation = deathAnim;
		break;
	}
	currentAnimation->SetCurrentFrame(skeletonNode.child("state").attribute("current_state").as_int());
	horizontalDirection = skeletonNode.child("state").attribute("horizontal_direction").as_int();


	return true;
}
// Saves the skeleton state
bool EnemySkeleton::SaveState(pugi::xml_node& skeletonNode) const
{
	// Save position
	pugi::xml_node skeleton = skeletonNode.append_child("position");
	skeleton.append_attribute("position_x").set_value(position.x);
	skeleton.append_attribute("position_y").set_value(position.y);

	// Save velocity
	skeleton = skeletonNode.append_child("velocity");
	skeleton.append_attribute("velocity_x").set_value(velocity.x);
	skeleton.append_attribute("velocity_y").set_value(velocity.y);

	// Save state info
	skeleton = skeletonNode.append_child("state");
	switch (state)
	{
	case SKELETON_IDLE:
		st = 0;
		break;
	case SKELETON_MOVE:
		st = 1;
		break;
	case SKELETON_ATTACK:
		st = 2;
		break;
	case SKELETON_DYING:
		st = 3;
		break;
	}
	skeleton.append_attribute("skeleton_state").set_value(st);
	skeleton.append_attribute("horizontal_direction").set_value(horizontalDirection);


	return true;
}


// Getter of the skeleton's rectangle
SDL_Rect EnemySkeleton::GetRect()
{
	return  { (int)position.x, (int)position.y, width, height };
}


// Check if the player is in range of the Skeleton
bool EnemySkeleton::Radar(fPoint playerPosition, int range)
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