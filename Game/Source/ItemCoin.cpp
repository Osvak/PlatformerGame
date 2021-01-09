#include "ItemCoin.h"

#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"
#include "Player.h"

#include "Log.h"
#include "Defs.h"



// Constructor
ItemCoin::ItemCoin(Render* render, Textures* tex, AudioManager* audioManager) : Entity(EntityType::ITEM_COIN)
{
	LOG("Creating Coin Entity");

	name.Create("coin");


	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;



	//
	// Animation pushbacks
	//
	coinAnim->loop = true;
	coinAnim->speed = 0.2f;
	for (int i = 0; i < 8; ++i)
	{
		coinAnim->PushBack({ 16 * i, 0, 16, 16 });
	}

	//
	// Load Potion texture file
	//
	coinTexture = tex->Load("Assets/Textures/Items/coin_spritesheet.png");

	//
	// Load Potion FX file
	//
	coinFX = audioManager->LoadFX("Assets/Audio/FX/coin.wav");
	audioManager->musicList.Add(&coinFX);

	//
	// Potion variables
	//
	isPickedUp = false;

}
// Destructor
ItemCoin::~ItemCoin()
{

}


// Checks the intersection between two rectangles
inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}
// Coin Update called every loop
bool ItemCoin::Update(float dt, Player* player)
{
	if (isPickedUp == false)
	{
		coinAnim->Update();

		if (CheckCollision(GetRect(), player->GetRect()) == true)
		{
			isPickedUp = true;
			audioManager->PlayFX(coinFX);
		}
	}


	return true;
}


// Draws the coin
bool ItemCoin::Draw()
{
	//
	// Draw Coin
	//
	if (isPickedUp == false)
	{
		SDL_Rect rect = coinAnim->GetCurrentFrame();
		render->DrawTexture(coinTexture, (int)position.x, (int)position.y, &rect);
	}


	return true;
}
void ItemCoin::DrawColliders()
{
	if (isPickedUp == false)
	{
		render->DrawRectangle(GetRect(), 255, 255, 0, 100);
	}
}


// Release memory
bool ItemCoin::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Unloading Coin");

	tex->UnLoad(coinTexture);
	delete coinAnim;
	coinAnim = nullptr;
	audioManager->UnloadFX(coinFX);

	active = false;

	return true;
}


// Loads the potion state
bool ItemCoin::LoadState(pugi::xml_node& coinNode)
{
	// Load position
	position.x = coinNode.child("position").attribute("position_x").as_float();
	position.y = coinNode.child("position").attribute("position_y").as_float();

	// Load state
	isPickedUp = coinNode.child("state").attribute("coin_state").as_bool();


	return true;
}
// Saves the potion state
bool ItemCoin::SaveState(pugi::xml_node& coinNode) const
{
	// Save position
	pugi::xml_node potion = coinNode.append_child("position");
	potion.append_attribute("position_x").set_value(position.x);
	potion.append_attribute("position_y").set_value(position.y);

	// Save state
	potion = coinNode.append_child("state");
	potion.append_attribute("coin_state").set_value(isPickedUp);


	return true;
}


// Getter for the potion's Rectangle
SDL_Rect ItemCoin::GetRect()
{
	return { (int)position.x, (int)position.y, width, height };
}