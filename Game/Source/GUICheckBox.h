#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GUIControl.h"

#include "Point.h"
#include "SString.h"

class GUICheckBox : public GUIControl
{
public:

	GUICheckBox(uint32 id, SDL_Rect bounds, const char* text, AudioManager* audioManager, Fonts* fonts, bool initState = false);
	virtual ~GUICheckBox();

	bool Update(Input* input, float dt);
	bool Draw(Render* render, bool drawGUI);

	bool GetValue() { return checked; };
	void SetValue(bool state) { checked = state; };

private:

	SDL_Rect FindPositionInAtlas();

private:

	AudioManager* audioManager;
	Fonts* fonts;

	// GUICheckBox specific properties
	unsigned int buttonFocusedFX = 0;
	unsigned int buttonPressedFX = 0;
	bool playFocusedFX = true;

	int guiFont = 0;

	bool checked;
};

#endif // !__GUICHECKBOX_H__