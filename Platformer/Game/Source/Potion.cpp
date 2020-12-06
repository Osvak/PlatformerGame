#include "Potion.h"

#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "Collisions.h"

#include "Log.h"

#include "SDL/include/SDL_rect.h"


// Constructor
Potion::Potion() : Module()
{
	name.Create("potion");
}

// Destructor
Potion::~Potion()
{

}

// Called when the module is activated
bool Potion::Awake()
{
	LOG("Loading Potion");

	bool ret = true;
	
	return ret;
}

bool Potion::Start()
{
	LOG("+++++ Loading potion textures");

	//
	// Load Lifes texture files 
	//
	potionTexture = app->tex->Load("Assets/textures/items/potion_sprite.png");

	//
	// Create Potion collider
	//
	potionCollider = app->collisions->AddCollider({ potionPosition.x, potionPosition.y, 8, 10 }, Collider::ColliderType::POTION, this);

	//
	// Set Flags and Variables
	//
	isCollected = false;
	potionPosition.x = TILE_SIZE * 41;
	potionPosition.y = TILE_SIZE * 13;

	return true;
}

bool Potion::Update(float dt)
{
	if (isCollected)
	{
		app->collisions->RemoveCollider(potionCollider);
	}

	return true;
}

bool Potion::PostUpdate()
{
	return true;
}

bool Potion::CleanUp()
{
	LOG("Freeing potion");

	if (!active)
	{
		return true;
	}
	
	app->tex->UnLoad(potionTexture);

	app->collisions->RemoveCollider(potionCollider);

	active = false;

	return true;
}
