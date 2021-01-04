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
}
// Destructor
EnemySkeleton::~EnemySkeleton()
{

}



// Skeleton Update
bool EnemySkeleton::Update(float dt)
{
	currentAnimation->Update();

	return true;
}

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
			render->DrawTexture(skeletonTexture, (int)position.x, (int)position.y, &rect);
		}
		if (horizontalDirection == -1)
		{
			render->DrawRectangle({ (int)position.x, (int)position.y, TILE_SIZE, TILE_SIZE }, 255, 0, 0);
			render->DrawFlippedTexture(skeletonTexture, (int)position.x - 8, (int)position.y - 21, &rect);
		}
	}


	return true;
}


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

bool EnemySkeleton::LoadState(pugi::xml_node& skeletonNode)
{


	return true;
}
bool EnemySkeleton::SaveState(pugi::xml_node& skeletonNode) const
{


	return true;
}