#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GUIControl.h"

#include "Point.h"
#include "SString.h"



class GUIButton : public GUIControl
{
public:

	GUIButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GUIButton();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);

private:

	// Gui Button specific properties
	// Maybe some animation properties for state change?

};

#endif // !__GUIBUTTON_H__