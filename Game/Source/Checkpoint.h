#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Entity.h"

#include "Animation.h"
#include "Point.h"
#include "SString.h"

#define CHECKPOINT_HEIGHT 20
#define CHECKPOINT_WIDTH 12


class Render;
class Textures;
class AudioManager;
class Player;

struct SDL_Texture;
struct SDL_Rect;

class Checkpoint : public Entity
{
public:

	// Constructor
	Checkpoint(Render* render, Textures* tex, AudioManager* audioManager);
	// Destructor
	virtual ~Checkpoint();


	// Called every loop
	bool Update(float dt, Player* player);


	// Draw the checkpoint
	bool Draw();
	void DrawColliders();


	// Release memory
	bool CleanUp();


	// Loads the checkpoint state
	bool LoadState(pugi::xml_node&) override;
	// Saves the checkpoint state
	bool SaveState(pugi::xml_node&) const override;


	// Getter for the Coin's Rectangle
	SDL_Rect GetRect();

public:

	// ----- CHECKPOINT VARIABLES ----- //
	int width = CHECKPOINT_WIDTH, height = CHECKPOINT_HEIGHT;
	fPoint position = { 0.0f, 0.0f };
	// ---------------------------- //

	// ----- CHECKPOINT FLAGS ----- //
	bool isActivated = false;
	// ------------------------ //

private:

	// ----- CHECKPOINT VARIABLES ----- //
	SDL_Texture* checkpointTexture = nullptr;
	// ---------------------------- //


	// ----- CHECKPOINT FLAGS ----- //
	bool playFX = true;
	// ------------------------ //

	// ----- ANIMATION SETS ----- //
	Animation* idleAnim = new Animation();
	Animation* activatedAnim = new Animation();
	Animation* currentAnimation = idleAnim;
	// -------------------------- //

	// ----- SOUND EFFECTS ----- //
	unsigned int checkpointFX = 0;
	// ------------------------- //

public:

	Render* render = nullptr;
	Textures* tex = nullptr;
	AudioManager* audioManager = nullptr;
	Player* player = nullptr;
};



#endif // !__CHECKPOINT_H__