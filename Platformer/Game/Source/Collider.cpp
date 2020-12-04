#include "Collider.h"

// Collider constructor
Collider::Collider(SDL_Rect rectangle, ColliderType type, Module* listener) : rect(rectangle), type(type)
{
	listeners[0] = listener;
}

// Set the position of a collider
void Collider::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

// Check if two collider squares are intersecting
bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}

// Add listener to the listener array
void Collider::AddListener(Module* listener)
{
	for (int i = 0; i < MAX_LISTENERS; ++i)
	{
		if (listeners[i] == nullptr)
		{
			listeners[i] = listener;
			break;
		}
		//Simple security check to avoid adding the same listener twice
		else if (listeners[i] == listener)
		{
			break;
		}
	}
}