#ifndef __ITEMPOTION_H__
#define __ITEMPOTION_H__

#include "Entity.h"

#include "Point.h"
#include "SString.h"

#define POTION_HEIGHT 10
#define POTION_WIDTH 8


class Render;
class Textures;
class AudioManager;
class Player;

struct SDL_Texture;
struct SDL_Rect;

class ItemPotion : public Entity
{
public:

	// Constructor
	ItemPotion(Render* render, Textures* tex, AudioManager* audioManager);
	// Destructor
	virtual ~ItemPotion();


	// Called every loop
	bool Update(float dt, Player* player);


	// Draw the potion
	bool Draw();
	void DrawColliders();


	// Release memory
	bool CleanUp();


	// Loads the Potion state
	bool LoadState(pugi::xml_node&) override;
	// Saves the Potion state
	bool SaveState(pugi::xml_node&) const override;


	// Getter for the potion's Rectangle
	SDL_Rect GetRect();

public:

	// ----- POTION VARIABLES ----- //
	int width = POTION_WIDTH, height = POTION_HEIGHT;
	fPoint position = { 0.0f, 0.0f };
	// ---------------------------- //

	// ----- POTION FLAGS ----- //
	bool isPickedUp = false;
	// ------------------------ //

private:

	// ----- POTION VARIABLES ----- //
	SDL_Texture* potionTexture = nullptr;
	// ---------------------------- //


	// ----- POTION FLAGS ----- //
	bool playFX = true;
	// ------------------------ //

	// ----- SOUND EFFECTS ----- //
	unsigned int potionFX = 0;
	// ------------------------- //

public:

	Render* render = nullptr;
	Textures* tex = nullptr;
	AudioManager* audioManager = nullptr;
	Player* player = nullptr;
};

#endif // !__ITEMPOTION_H__