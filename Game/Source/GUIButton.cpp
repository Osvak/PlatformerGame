#include "GUIButton.h"



// Constructor
GUIButton::GUIButton(uint32 id, SDL_Rect bounds, const char* text, AudioManager* audioManager, Fonts* fonts) : GUIControl(GUIControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
    this->audioManager = audioManager;
    this->fonts = fonts;



    buttonFocusedFX = audioManager->LoadFX("Assets/Audio/FX/button_focused.wav");
    audioManager->musicList.Add(&buttonFocusedFX);
    buttonPressedFX = audioManager->LoadFX("Assets/Audio/FX/button_pressed.wav");
    audioManager->musicList.Add(&buttonPressedFX);

    guiFont = fonts->Load("Assets/Fonts/font_black.png", "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ", 2, 189, 18);
}
// Destructor
GUIButton::~GUIButton()
{
    audioManager->UnloadFX(buttonFocusedFX);
    audioManager->UnloadFX(buttonPressedFX);
    fonts->UnLoad(guiFont);
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
            if (playFocusedFX == true)
            {
                audioManager->PlayFX(buttonFocusedFX);
                playFocusedFX = false;
            }
            
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GUIControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                audioManager->PlayFX(buttonPressedFX);
                NotifyObserver();
            }
        }
        else
        {
            state = GUIControlState::NORMAL;
            playFocusedFX = true;
        }
    }

    return false;
}

bool GUIButton::Draw(Render* render, bool drawGUI)
{
    SDL_Rect rect = FindPositionInAtlas();

    // Draw the right button depending on state
    switch (state)
    {
    case GUIControlState::DISABLED:
    {
        render->DrawTexture(texture, bounds.x, bounds.y, &rect);

        if (drawGUI == true)
        {
            render->DrawRectangle(bounds, 100, 100, 100, 150);
        }
      

        break;
    }

    case GUIControlState::NORMAL:
    {
        render->DrawTexture(texture, bounds.x, bounds.y, &rect);

        if (drawGUI == true)
        {
            render->DrawRectangle(bounds, 0, 255, 0, 150);
        }


        break;
    }

    case GUIControlState::FOCUSED:
    {
        render->DrawTexture(texture, bounds.x, bounds.y, &rect);

        if (drawGUI == true)
        {
            render->DrawRectangle(bounds, 255, 255, 0, 150);
        }


        break;
    }

    case GUIControlState::PRESSED:
    {
        render->DrawTexture(texture, bounds.x, bounds.y, &rect);

        if (drawGUI == true)
        {
            render->DrawRectangle(bounds, 0, 255, 255, 150);
        }


        break;
    }

    default:
        break;
    }

    // Draw text of the button
    int posX = (int)(bounds.x + (bounds.w / 2) - ((float)text.Length() / 2 ) * 8.5f);
    int posY = bounds.y + (int)(bounds.h / 2) - 5;
    fonts->DrawText(posX, posY, guiFont, text.GetString());


    return false;
}


SDL_Rect GUIButton::FindPositionInAtlas()
{
    SDL_Rect rect = { 0, 0, 0, 0 };

    switch (state)
    {
    case GUIControlState::DISABLED:
        rect = { 103, 144, 100, 24 };
        break;
    case GUIControlState::NORMAL:
        rect = { 103, 27, 100, 24 };
        break;
    case GUIControlState::FOCUSED:
        rect = { 103, 66, 100, 24 };
        break;
    case GUIControlState::PRESSED:
        rect = { 103, 105, 100, 24 };
        break;
    default:
        break;
    }


    return rect;
}