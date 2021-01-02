#include "Collisions.h"

#include "Input.h"
#include "Render.h"

#include "Log.h"


// Constructor, fills the collision matrix data
Collisions::Collisions(Input* input, Render* render)
{
	this->input = input;
	this->render = render;


	//
	// Colliders matrix
	//
	// Player collider control
	matrix[Collider::ColliderType::PLAYER][Collider::ColliderType::PLAYER] = false;
	matrix[Collider::ColliderType::PLAYER][Collider::ColliderType::WALL] = true;
	matrix[Collider::ColliderType::PLAYER][Collider::ColliderType::PLATFORM] = true;
	matrix[Collider::ColliderType::PLAYER][Collider::ColliderType::DIE] = true;
	matrix[Collider::ColliderType::PLAYER][Collider::ColliderType::NEXT_LEVEL] = true;
	matrix[Collider::ColliderType::PLAYER][Collider::ColliderType::GOD_MODE] = false;
	matrix[Collider::ColliderType::PLAYER][Collider::ColliderType::CHECKPOINT] = true;
	matrix[Collider::ColliderType::PLAYER][Collider::ColliderType::CAMERA_WINDOW] = true;
	matrix[Collider::ColliderType::PLAYER][Collider::ColliderType::POTION] = true;


	// Platform collider control
	matrix[Collider::ColliderType::PLATFORM][Collider::ColliderType::PLAYER] = true;
	matrix[Collider::ColliderType::PLATFORM][Collider::ColliderType::WALL] = false;
	matrix[Collider::ColliderType::PLATFORM][Collider::ColliderType::PLATFORM] = false;
	matrix[Collider::ColliderType::PLATFORM][Collider::ColliderType::DIE] = false;
	matrix[Collider::ColliderType::PLATFORM][Collider::ColliderType::NEXT_LEVEL] = false;
	matrix[Collider::ColliderType::PLATFORM][Collider::ColliderType::GOD_MODE] = false;
	matrix[Collider::ColliderType::PLATFORM][Collider::ColliderType::CHECKPOINT] = false;
	matrix[Collider::ColliderType::PLATFORM][Collider::ColliderType::CAMERA_WINDOW] = false;
	matrix[Collider::ColliderType::PLATFORM][Collider::ColliderType::POTION] = false;


	// Wall collider control
	matrix[Collider::ColliderType::WALL][Collider::ColliderType::PLAYER] = true;
	matrix[Collider::ColliderType::WALL][Collider::ColliderType::WALL] = false;
	matrix[Collider::ColliderType::WALL][Collider::ColliderType::PLATFORM] = false;
	matrix[Collider::ColliderType::WALL][Collider::ColliderType::DIE] = false;
	matrix[Collider::ColliderType::WALL][Collider::ColliderType::NEXT_LEVEL] = false;
	matrix[Collider::ColliderType::WALL][Collider::ColliderType::GOD_MODE] = false;
	matrix[Collider::ColliderType::WALL][Collider::ColliderType::CHECKPOINT] = false;
	matrix[Collider::ColliderType::WALL][Collider::ColliderType::CAMERA_WINDOW] = false;
	matrix[Collider::ColliderType::WALL][Collider::ColliderType::POTION] = false;


	// Die collider control
	matrix[Collider::ColliderType::DIE][Collider::ColliderType::PLAYER] = true;
	matrix[Collider::ColliderType::DIE][Collider::ColliderType::WALL] = false;
	matrix[Collider::ColliderType::DIE][Collider::ColliderType::PLATFORM] = false;
	matrix[Collider::ColliderType::DIE][Collider::ColliderType::DIE] = false;
	matrix[Collider::ColliderType::DIE][Collider::ColliderType::NEXT_LEVEL] = false;
	matrix[Collider::ColliderType::DIE][Collider::ColliderType::GOD_MODE] = false;
	matrix[Collider::ColliderType::DIE][Collider::ColliderType::CHECKPOINT] = false;
	matrix[Collider::ColliderType::DIE][Collider::ColliderType::CAMERA_WINDOW] = false;
	matrix[Collider::ColliderType::DIE][Collider::ColliderType::POTION] = false;


	// Next Level collider control
	matrix[Collider::ColliderType::NEXT_LEVEL][Collider::ColliderType::PLAYER] = true;
	matrix[Collider::ColliderType::NEXT_LEVEL][Collider::ColliderType::WALL] = false;
	matrix[Collider::ColliderType::NEXT_LEVEL][Collider::ColliderType::PLATFORM] = false;
	matrix[Collider::ColliderType::NEXT_LEVEL][Collider::ColliderType::DIE] = false;
	matrix[Collider::ColliderType::NEXT_LEVEL][Collider::ColliderType::NEXT_LEVEL] = false;
	matrix[Collider::ColliderType::NEXT_LEVEL][Collider::ColliderType::GOD_MODE] = false;
	matrix[Collider::ColliderType::NEXT_LEVEL][Collider::ColliderType::CHECKPOINT] = false;
	matrix[Collider::ColliderType::NEXT_LEVEL][Collider::ColliderType::CAMERA_WINDOW] = false;
	matrix[Collider::ColliderType::NEXT_LEVEL][Collider::ColliderType::POTION] = false;


	// God mode collider control
	matrix[Collider::ColliderType::GOD_MODE][Collider::ColliderType::PLAYER] = false;
	matrix[Collider::ColliderType::GOD_MODE][Collider::ColliderType::WALL] = false;
	matrix[Collider::ColliderType::GOD_MODE][Collider::ColliderType::PLATFORM] = false;
	matrix[Collider::ColliderType::GOD_MODE][Collider::ColliderType::DIE] = false;
	matrix[Collider::ColliderType::GOD_MODE][Collider::ColliderType::NEXT_LEVEL] = false;
	matrix[Collider::ColliderType::GOD_MODE][Collider::ColliderType::GOD_MODE] = false;
	matrix[Collider::ColliderType::GOD_MODE][Collider::ColliderType::CHECKPOINT] = false;
	matrix[Collider::ColliderType::GOD_MODE][Collider::ColliderType::CAMERA_WINDOW] = true;	
	matrix[Collider::ColliderType::GOD_MODE][Collider::ColliderType::POTION] = false;

	// Checkpoint collider control
	matrix[Collider::ColliderType::CHECKPOINT][Collider::ColliderType::PLAYER] = true;
	matrix[Collider::ColliderType::CHECKPOINT][Collider::ColliderType::WALL] = false;
	matrix[Collider::ColliderType::CHECKPOINT][Collider::ColliderType::PLATFORM] = false;
	matrix[Collider::ColliderType::CHECKPOINT][Collider::ColliderType::DIE] = false;
	matrix[Collider::ColliderType::CHECKPOINT][Collider::ColliderType::NEXT_LEVEL] = false;
	matrix[Collider::ColliderType::CHECKPOINT][Collider::ColliderType::CHECKPOINT] = false;
	matrix[Collider::ColliderType::CHECKPOINT][Collider::ColliderType::GOD_MODE] = false;
	matrix[Collider::ColliderType::CHECKPOINT][Collider::ColliderType::CAMERA_WINDOW] = false;
	matrix[Collider::ColliderType::CHECKPOINT][Collider::ColliderType::POTION] = false;

	// Camera window collider control
	matrix[Collider::ColliderType::CAMERA_WINDOW][Collider::ColliderType::PLAYER] = true;
	matrix[Collider::ColliderType::CAMERA_WINDOW][Collider::ColliderType::WALL] = false;
	matrix[Collider::ColliderType::CAMERA_WINDOW][Collider::ColliderType::PLATFORM] = false;
	matrix[Collider::ColliderType::CAMERA_WINDOW][Collider::ColliderType::DIE] = false;
	matrix[Collider::ColliderType::CAMERA_WINDOW][Collider::ColliderType::NEXT_LEVEL] = false;
	matrix[Collider::ColliderType::CAMERA_WINDOW][Collider::ColliderType::GOD_MODE] = true;
	matrix[Collider::ColliderType::CAMERA_WINDOW][Collider::ColliderType::CHECKPOINT] = false;
	matrix[Collider::ColliderType::CAMERA_WINDOW][Collider::ColliderType::CAMERA_WINDOW] = false;
	matrix[Collider::ColliderType::CAMERA_WINDOW][Collider::ColliderType::POTION] = false;

	// Potion collider control
	matrix[Collider::ColliderType::POTION][Collider::ColliderType::PLAYER] = true;
	matrix[Collider::ColliderType::POTION][Collider::ColliderType::WALL] = false;
	matrix[Collider::ColliderType::POTION][Collider::ColliderType::PLATFORM] = false;
	matrix[Collider::ColliderType::POTION][Collider::ColliderType::DIE] = false;
	matrix[Collider::ColliderType::POTION][Collider::ColliderType::NEXT_LEVEL] = false;
	matrix[Collider::ColliderType::POTION][Collider::ColliderType::GOD_MODE] = false;
	matrix[Collider::ColliderType::POTION][Collider::ColliderType::CHECKPOINT] = false;
	matrix[Collider::ColliderType::POTION][Collider::ColliderType::CAMERA_WINDOW] = false;
	matrix[Collider::ColliderType::POTION][Collider::ColliderType::POTION] = false;
}

// Destructor
Collisions::~Collisions()
{

}

// Called before the Updates
bool Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (matrix[c1->type][c2->type] && c1->CheckCollision(c2->rect))
			{
				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c1->listeners[i] != nullptr) c1->listeners[i]->OnCollision(c1, c2);

				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c2->listeners[i] != nullptr) c2->listeners[i]->OnCollision(c2, c1);
			}
		}
	}

	return true;
}

// Check if you want to Debug
bool Collisions::Update(float dt)
{
	if (input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	return true;
}

// Draw the Debug Collision squares
bool Collisions::PostUpdate()
{
	if (debug == true)
	{
		DebugDraw();
	}

	return true;
}

// Called before quitting
bool Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}

	return true;
}

// Draw the squares for each collider
void Collisions::DebugDraw()
{
	Uint8 alpha = 100;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
			//
			// Color of each collider
			//
		case Collider::ColliderType::NONE:
			render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha); // white
			break;
		case Collider::ColliderType::PLAYER:
			render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha); // green
			break;
		case Collider::ColliderType::WALL:
			render->DrawRectangle(colliders[i]->rect, 105, 85, 157, alpha); // gray magenta
			break;
		case Collider::ColliderType::PLATFORM:
			render->DrawRectangle(colliders[i]->rect, 0, 230, 185, alpha); // mint
			break;
		case Collider::ColliderType::DIE:
			render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha); // red
			break;
		case Collider::ColliderType::NEXT_LEVEL:
			render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha); // blue
			break;

		case Collider::ColliderType::GOD_MODE:
			render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha); // white
			break;
		case Collider::ColliderType::CHECKPOINT:
			render->DrawRectangle(colliders[i]->rect, 100, 100, 0, alpha); // green camo
		case Collider::ColliderType::CAMERA_WINDOW:
			render->DrawRectangle(colliders[i]->rect, 250, 110, 190, alpha); // bubble gum pink
			break;
		case Collider::ColliderType::POTION:
			render->DrawRectangle(colliders[i]->rect, 0, 255, 255, alpha); //green blue
		}
		//printf("Tile id %d with Collider Type %d in (%d,%d)\n", i, colliders[i]->type, colliders[i]->rect.x, colliders[i]->rect.y); // Debug Collider list
	}
}

// Add Collider to the Collider Array
Collider* Collisions::AddCollider(SDL_Rect rect, Collider::ColliderType type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			++colliderCount;
			break;
		}
	}

	return ret;
}

// Remove collider from the Collider Array
void Collisions::RemoveCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
			break;
		}
	}
}