#ifndef __ITEMCOIN_H__
#define __ITEMCOIN_H__

#include "Entity.h"

#include "Animation.h"
#include "Point.h"
#include "SString.h"

#define COIN_HEIGHT 10
#define COIN_WIDTH 8


class Render;
class Textures;
class AudioManager;
class Player;

struct SDL_Texture;
struct SDL_Rect;

class ItemCoin : public Entity
{
public:

	// Constructor
	ItemCoin(Render* render, Textures* tex, AudioManager* audioManager);
	// Destructor
	virtual ~ItemCoin();


	// Called every loop
	bool Update(float dt, Player* player);


	// Draw the coin
	bool Draw();
	void DrawColliders();


	// Release memory
	bool CleanUp();


	// Loads the Coin state
	bool LoadState(pugi::xml_node&) override;
	// Saves the Coin state
	bool SaveState(pugi::xml_node&) const override;


	// Getter for the Coin's Rectangle
	SDL_Rect GetRect();

public:

	// ----- COIN VARIABLES ----- //
	int width = COIN_WIDTH, height = COIN_HEIGHT;
	fPoint position = { 0.0f, 0.0f };
	// ---------------------------- //

	// ----- COIN FLAGS ----- //
	bool isPickedUp = false;
	// ------------------------ //

private:

	// ----- COIN VARIABLES ----- //
	SDL_Texture* coinTexture = nullptr;
	// ---------------------------- //


	// ----- COIN FLAGS ----- //
	bool playFX = true;
	// ------------------------ //

	// ----- ANIMATION SETS ----- //
	Animation* coinAnim = new Animation();
	// -------------------------- //

	// ----- SOUND EFFECTS ----- //
	unsigned int coinFX = 0;
	// ------------------------- //

public:

	Render* render = nullptr;
	Textures* tex = nullptr;
	AudioManager* audioManager = nullptr;
	Player* player = nullptr;
};

#endif // !__ITEMCOIN_H__