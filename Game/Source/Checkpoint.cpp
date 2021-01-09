#include "Checkpoint.h"

#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "Player.h"

#include "Log.h"
#include "Defs.h"



// Constructor
Checkpoint::Checkpoint(Render* render, Textures* tex, AudioManager* audioManager) : Entity(EntityType::CHECKPOINT)
{
	LOG("Creating Checkpoint Entity");

	name.Create("checkpoint");


	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;



	//
	// Animation pushbacks
	//
	idleAnim->loop = true;
	idleAnim->speed = 0.1f;
	for (int i = 0; i < 9; ++i)
	{
		idleAnim->PushBack({ 12 * i, 0, 12, 20 });
	}
	activatedAnim->loop = true;
	activatedAnim->speed = 0.1f;
	for (int i = 0; i < 3; ++i)
	{
		activatedAnim->PushBack({ 12 * i, 20, 12, 20 });
	}

	//
	// Load Checkpoint texture file
	//
	checkpointTexture = tex->Load("Assets/Textures/Items/checkpoint_spritesheet.png");
	checkpointMenus = tex->Load("Assets/Textures/Items/checkpoint_menu_sprites.png");

	//
	// Load Checkpoint FX file
	//
	checkpointFX = audioManager->LoadFX("Assets/Audio/FX/checkpoint.wav");
	audioManager->musicList.Add(&checkpointFX);

	//
	// Checkpoint variables
	//
	isActivated = false;
	currentAnimation = idleAnim;

}
// Destructor
Checkpoint::~Checkpoint()
{

}


// Checks the intersection between two rectangles
inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}
// Checkpoint Update called every loop
bool Checkpoint::Update(float dt, Player* player)
{
	if (isActivated == false)
	{
		currentAnimation->Update();

		if (CheckCollision(GetRect(), player->GetRect()))
		{
			currentAnimation = activatedAnim;
			isActivated = true;
			audioManager->PlayFX(checkpointFX);
		}
	}
	else
	{
		currentAnimation->Update();

		if (CheckCollision({ (int)position.x - width, (int)position.y - height, 3 * width, 3 * height }, player->GetRect()))
		{
			playerNearby = true;
		}
		else
		{
			playerNearby = false;
		}
	}

	
	return true;
}


// Draws the checpoint
bool Checkpoint::Draw()
{
	//
	// Draw checkpoint
	//
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	render->DrawTexture(checkpointTexture, (int)position.x, (int)position.y, &rect);

	if (playerNearby == true && travelling == false)
	{
		SDL_Rect rect = { 0, 0, 40, 32 };
		render->DrawTexture(checkpointMenus, (int)position.x - 14, (int)position.y - 14, &rect);
	}
	if (travelling == true)
	{
		SDL_Rect rect = { 40, 0, 40, 32 };
		render->DrawTexture(checkpointMenus, (int)position.x - 14, (int)position.y - 14, &rect);
	}

	return true;
}
void Checkpoint::DrawColliders()
{
	render->DrawRectangle(GetRect(), 125, 5, 215, 100);
}


// Release memory
bool Checkpoint::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Unloading Checkpoint");

	tex->UnLoad(checkpointTexture);
	tex->UnLoad(checkpointMenus);

	audioManager->UnloadFX(checkpointFX);

	active = false;

	return true;
}


// Loads the checkpoint state
bool Checkpoint::LoadState(pugi::xml_node& coinNode)
{
	// Load position
	position.x = coinNode.child("position").attribute("position_x").as_float();
	position.y = coinNode.child("position").attribute("position_y").as_float();

	// Load state
	isActivated = coinNode.child("state").attribute("checkpoint_state").as_bool();


	return true;
}
// Saves the potion state
bool Checkpoint::SaveState(pugi::xml_node& coinNode) const
{
	// Save position
	pugi::xml_node potion = coinNode.append_child("position");
	potion.append_attribute("position_x").set_value(position.x);
	potion.append_attribute("position_y").set_value(position.y);

	// Save state
	potion = coinNode.append_child("state");
	potion.append_attribute("checkpoint_state").set_value(isActivated);


	return true;
}


// Getter for the checkpoint's Rectangle
SDL_Rect Checkpoint::GetRect()
{
	return { (int)position.x, (int)position.y, width, height };
}