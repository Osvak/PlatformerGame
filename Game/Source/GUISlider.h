#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GUIControl.h"

#include "Point.h"
#include "SString.h"

#define SPACEBOUNDBAR 10


class GUISlider : public GUIControl
{
public:

	GUISlider(uint32 id, SDL_Rect bounds, AudioManager* audioManager, Fonts* fonts, const char* text, int initialValue = 0, int min = 0, int max = 100);
	virtual ~GUISlider();

	bool Update(Input* input, float dt);
	bool Draw(Render* render, bool drawGUI);

	void SliderControl(int mouseX, int mouseY);

	int GetValue() { return value; };

	// Put the slider in the correct position respected the value && change the value
	void SetValue(int newValue);

private:

	SDL_Rect FindPositionInAtlas();

private:

	AudioManager* audioManager;
	Fonts* fonts;

	// GUISlider specific properties
	// Maybe some animation properties for state change?

	SDL_Rect sliderButton; // Pressable part
	SDL_Rect sliderBar; // Back part

	int guiFont = 0;

	int lastValue;
	int minValue;
	int maxValue;

};

#endif // !__GUISLIDER_H__