#include "GUIButton.h"



// Constructor
GUIButton::GUIButton(uint32 id, SDL_Rect bounds, const char* text) : GUIControl(GUIControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
}
// Destructor
GUIButton::~GUIButton()
{

}


// Called each loop
bool GUIButton::Update(Input* input, float dt)
{
    if (state != GUIControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GUIControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GUIControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                NotifyObserver();
            }
        }
        else state = GUIControlState::NORMAL;
    }

    return false;
}

bool GUIButton::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GUIControlState::DISABLED: render->DrawRectangle(bounds, 100, 100, 100, 255);
        break;
    case GUIControlState::NORMAL: render->DrawRectangle(bounds, 0, 255, 0, 255);
        break;
    case GUIControlState::FOCUSED: render->DrawRectangle(bounds, 255, 255, 0, 255);
        break;
    case GUIControlState::PRESSED: render->DrawRectangle(bounds, 0, 255, 255, 255);
        break;
    default:
        break;
    }

    return false;
}