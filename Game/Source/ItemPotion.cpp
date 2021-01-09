#include "ItemPotion.h"

#include "Render.h"
#include "Textures.h"
#include "AudioManager.h"

#include "Log.h"
#include "Defs.h"



// Constructor
ItemPotion::ItemPotion(Render* render, Textures* tex, AudioManager* audioManager) : Entity(EntityType::ITEM_POTION)
{
	LOG("Creating Potion Entity");

	name.Create("potion");


	this->render = render;
	this->tex = tex;
	this->audioManager = audioManager;



	//
	// Load Potion texture file
	//
	potionTexture = tex->Load("Assets/Textures/Items/potion_sprite.png");
}
// Destructor
ItemPotion::~ItemPotion()
{

}


// Checks the intersection between two rectangles
inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}
// Potion Update called every loop
bool ItemPotion::Update(float dt, Player* player)
{
	if (isPickedUp == false)
	{
		this->player = player;
		
	}
}


// Draws the potion
bool ItemPotion::Draw()
{
	//
	// Draw Potion
	//
	if (isPickedUp == false)
	{
		render->DrawTexture(potionTexture, (int)position.x, (int)position.y);
	}
}
void ItemPotion::DrawColliders()
{
	if (isPickedUp == false)
	{
		render->DrawRectangle({ (int)position.x, (int)position.y, width, height }, 69, 245, 80, 100);
	}
}


// Release memory
bool ItemPotion::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Unloading Ghost");

	tex->UnLoad(potionTexture);

	active = false;

	return true;
}


// Loads the potion state
bool ItemPotion::LoadState(pugi::xml_node& potionNode)
{
	// Load position
	position.x = potionNode.child("position").attribute("position_x").as_float();
	position.y = potionNode.child("position").attribute("position_y").as_float();
	
	// Load state
	isPickedUp = potionNode.child("state").attribute("potion_state").as_bool();


	return true;
}
bool ItemPotion::SaveState(pugi::xml_node& potionNode) const
{
	// Save position
	pugi::xml_node potion = potionNode.append_child("position");
	potion.append_attribute("position_x").set_value(position.x);
	potion.append_attribute("position_y").set_value(position.y);

	// Save state
	potion = potionNode.append_child("state");
	potion.append_attribute("potion_state").set_value(isPickedUp);


	return true;
}