#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GUIControl.h"

#include "Point.h"
#include "SString.h"



class GUIButton : public GUIControl
{
public:

	GUIButton(uint32 id, SDL_Rect bounds, const char* text, AudioManager* audioManager);
	virtual ~GUIButton();

	bool Update(Input* input, float dt);
	bool Draw(Render* render, bool drawGUI);

private:

	SDL_Rect FindPositionInAtlas();

private:

	AudioManager* audioManager;

	// Gui Button specific properties
	// Maybe some animation properties for state change?
	unsigned int buttonFocusedFX = 0;
	unsigned int buttonPressedFX = 0;
	bool playFocusedFX = true;

};

#endif // !__GUIBUTTON_H__