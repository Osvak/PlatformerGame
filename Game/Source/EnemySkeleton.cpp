#include "EnemySkeleton.h"

#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"

#include "Log.h"
#include "Defs.h"



EnemySkeleton::EnemySkeleton(Render* render, Textures* tex, AudioManager* audioManager) : Entity(EntityType::ENEMY_SKELETON)
{
	LOG("Creating Skeleton Entity");

	name.Create("skeleton");


	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;


	//
	// Animation pushbacks
	//
	attackAnim->loop = false;
	attackAnim->speed = 0.2f;
	for (int i = 0; i < 18; ++i)
	{
		attackAnim->PushBack({ 43 * i, 37, 43, 37 });
	}
	idleAnim->loop = true;
	idleAnim->pingPong = true;
	idleAnim->speed = 0.1f;
	for (int i = 0; i < 11; ++i)
	{
		idleAnim->PushBack({ 24 * i, 37, 24, 37 });
	}
	walkAnim->loop = true;
	walkAnim->speed = 0.2f;
	for (int i = 0; i < 13; ++i)
	{
		walkAnim->PushBack({ 24 * i, 37, 24, 37 });
	}
	deathAnim->loop = true;
	deathAnim->speed = 0.2f;
	for (int i = 0; i < 15; ++i)
	{
		deathAnim->PushBack({ 32 * i, 37, 32, 37 });
	}

	//
	// Load Skeleton texture files
	//
	skeletonTexture = tex->Load("Assets/Textures/Enemies/skeleton_spritesheet.png");

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
}
// Destructor
EnemySkeleton::~EnemySkeleton()
{

}


// Skeleton Update called every loop
bool EnemySkeleton::Update(float dt)
{
	currentAnimation->Update();

	return true;
}

// Draws the skeleton
bool EnemySkeleton::Draw()
{
	//
	// Draw Skeleton
	//
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();

		if (horizontalDirection == 1)
		{
			render->DrawTexture(skeletonTexture, (int)position.x - 11, (int)position.y - 15, &rect);
		}
		if (horizontalDirection == -1)
		{
			render->DrawFlippedTexture(skeletonTexture, (int)position.x - 11, (int)position.y - 15, &rect);
		}
	}


	return true;
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


SDL_Rect EnemySkeleton::GetRect()
{
	return  { (int)position.x, (int)position.y, width, height };
}