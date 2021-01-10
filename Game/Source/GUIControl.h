#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "AudioManager.h"
#include "Fonts.h"

#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

enum class GUIControlType
{
    BUTTON,
    CHECKBOX,
    SLIDER
};

enum class GUIControlState
{
    DISABLED,
    NORMAL,
    FOCUSED,
    PRESSED
};

class GUIControl
{
public:

    GUIControl(GUIControlType type, uint32 id) : type(type), id(id), state(GUIControlState::NORMAL) {}

    GUIControl(GUIControlType type, SDL_Rect bounds, const char* text, AudioManager* audioManager, Fonts* fonts) :
        type(type),
        state(GUIControlState::NORMAL),
        bounds(bounds),
        text(text)
    {
        color.r = 255; color.g = 255; color.b = 255;
        texture = NULL;
    }

    virtual bool Update(Input* input, float dt)
    {
        return true;
    }

    virtual bool Draw(Render* render, bool drawGUI) const
    {
        return true;
    }

    void SetTexture(SDL_Texture* tex)
    {
        texture = tex;
        section = { 0, 0, 0, 0 };
    }

    void SetObserver(Scene* module)
    {
        observer = module;
    }

    void NotifyObserver()
    {
        observer->OnGUIMouseClickEvent(this);
    }

public:

    int value;

    uint32 id;
    GUIControlType type;
    GUIControlState state;

    SString text;           // Control text (if required)
    SDL_Rect bounds;        // Position and size
    SDL_Color color;        // Tint color

    SDL_Texture* texture;   // Texture atlas reference
    SDL_Rect section;       // Texture atlas base section

    //Font font;              // Text font

    Scene* observer;        // Observer module (it should probably be an array/list)
};

#endif // __GUICONTROL_H__