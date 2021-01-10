#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GUIControl.h"

#include "Point.h"
#include "SString.h"



class GUIButton : public GUIControl
{
public:

	GUIButton(uint32 id, SDL_Rect bounds, const char* text, AudioManager* audioManager, Fonts* fonts);
	virtual ~GUIButton();

	bool Update(Input* input, float dt);
	bool Draw(Render* render, bool drawGUI);

private:

	SDL_Rect FindPositionInAtlas();

private:

	AudioManager* audioManager;
	Fonts* fonts;

	// Gui Button specific properties
	// Maybe some animation properties for state change?
	unsigned int buttonFocusedFX = 0;
	unsigned int buttonPressedFX = 0;
	bool playFocusedFX = true;

	int guiFont = 0;

};

#endif // !__GUIBUTTON_H__