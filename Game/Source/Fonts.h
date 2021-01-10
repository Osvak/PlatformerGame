#ifndef __MODULE_FONTS_H__
#define __MODULE_FONTS_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256



class Render;
class Textures;

struct SDL_Texture;

struct Font
{
	// Lookup table. All characters displayed in the same order as the texture
	char table[MAX_FONT_CHARS];

	// The font texture
	SDL_Texture* texture = nullptr;

	// Font setup data
	uint totalLength = 0;
	uint rows, columns = 0;
	float char_w, char_h = 0;
};

class Fonts : public Module
{
public:

	// Constructor
	Fonts(Render* render, Textures* tex);

	// Destructor
	~Fonts();

	bool CleanUp();

	// Loads a font file from a texture
	// Returns a font index from the fonts array
	// Param texturePath	- The path to the texture file
	// Param characters		- The lookup table. All characters displayed in the same order as the texture
	// Param rows			- The amount of character rows in the texture
	// Param image_w		- width of the texture
	// Param image_h		- height of the texture
	int Load(const char* texturePath, const char* characters, uint rows, int image_w, int image_h);

	// Removes a font by its index
	// Unloads the texture and removes it from the fonts array
	void UnLoad(int fontIndex);

	// Create a surface from text
	void DrawText(int x, int y, int fontIndex, const char* text, bool greyText = false) const;


private:
	// An array to keep track and store all loaded fonts
	Font fonts[MAX_FONTS];

	Render* render;
	Textures* tex;
};

#endif // __ModuleFonts_H__