#include "GUISlider.h"



// Constructor
GUISlider::GUISlider(uint32 id, SDL_Rect bounds, AudioManager* audioManager, Fonts* fonts, const char* text, int min, int max) : GUIControl(GUIControlType::SLIDER, id)
{
	this->sliderBar = bounds;
	this->text = text;
	if (min > max)
	{
		this->minValue = max;
		this->maxValue = min;
	}
	else
	{
		this->minValue = min;
		this->maxValue = max;
	}
	this->audioManager = audioManager;
	this->fonts = fonts;

	sliderButton.w = 10;
	sliderButton.h = 6;
	sliderButton.y = sliderBar.y + ((sliderBar.h / 2) - 3);
	sliderButton.x = sliderBar.x + 2;


	guiFont = fonts->Load("Assets/Fonts/font_white.png", "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ", 2, 189, 18);
}
// Destructor
GUISlider::~GUISlider()
{
	fonts->UnLoad(guiFont);
}


// Called each loop
bool GUISlider::Update(Input* input, float dt)
{
	if (state != GUIControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		// Check collision between mouse and button bounds
		if (((mouseX > sliderBar.x) && (mouseX < (sliderBar.x + sliderBar.w)) &&
			(mouseY > sliderBar.y) && (mouseY < (sliderBar.y + sliderBar.h))) ||
			((mouseX > sliderButton.x) && (mouseX < (sliderButton.x + sliderButton.w)) &&
				(mouseY > sliderButton.y) && (mouseY < (sliderButton.y + sliderButton.h))))
		{
			state = GUIControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GUIControlState::PRESSED;
			}

			if (state == GUIControlState::PRESSED)
			{
				SliderControl(mouseX, mouseY);
				NotifyObserver();
			}
			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				

			}
		}
		else
		{
			state = GUIControlState::NORMAL;
		}
	}

	return false;
}

bool GUISlider::Draw(Render* render, bool drawGUI)
{
	SDL_Rect sliderTexturePosition = FindPositionInAtlas();
	SDL_Rect buttonTexturePosition = { 422, 47, 10, 6 };

	switch (state)
	{
	case GUIControlState::DISABLED:
	{
		render->DrawTexture(texture, sliderBar.x, sliderBar.y, &sliderTexturePosition);
		
		if (drawGUI == true)
		{
			render->DrawRectangle(sliderBar, 100, 100, 100, 150);
		}


		break;
	}

	case GUIControlState::NORMAL:
	{
		render->DrawTexture(texture, sliderBar.x, sliderBar.y, &sliderTexturePosition);
		render->DrawTexture(texture, sliderButton.x, sliderButton.y, &buttonTexturePosition);

		if (drawGUI == true)
		{
			render->DrawRectangle(sliderBar, 0, 255, 0, 150);
			render->DrawRectangle(sliderButton, 0, 255, 0, 150);
		}


		break;
	}

	case GUIControlState::FOCUSED:
	{
		render->DrawTexture(texture, sliderBar.x, sliderBar.y, &sliderTexturePosition);
		render->DrawTexture(texture, sliderButton.x, sliderButton.y, &buttonTexturePosition);


		if (drawGUI == true)
		{
			render->DrawRectangle(sliderBar, 255, 255, 0, 150);
			render->DrawRectangle(sliderButton, 255, 255, 0, 150);
		}


		break;
	}

	case GUIControlState::PRESSED:
	{
		render->DrawTexture(texture, sliderBar.x, sliderBar.y, &sliderTexturePosition);
		render->DrawTexture(texture, sliderButton.x, sliderButton.y, &buttonTexturePosition);

		if (drawGUI == true)
		{
			render->DrawRectangle(sliderBar, 0, 255, 255, 150);
		}
		

		break;
	}

	default:
		break;
	}


	// Draw name of the slider
	int posX = sliderBar.x + ((int)((float)sliderBar.w / 2));
	posX = posX - (int)((((float)text.Length() / 2) + 0.5f) * 8);
	int posY = sliderBar.y - 10;
	fonts->DrawText(posX, posY, guiFont, text.GetString());


	return false;
}

SDL_Rect GUISlider::FindPositionInAtlas()
{
	SDL_Rect rect = { 0, 0, 0, 0 };

	switch (state)
	{
	case GUIControlState::DISABLED:
		rect = { 421, 151, 100, 10 };
		break;
	case GUIControlState::NORMAL:
		rect = { 421, 34, 100, 10 };
		break;
	case GUIControlState::FOCUSED:
		rect = { 421, 73, 100, 10 };
		break;
	case GUIControlState::PRESSED:
		rect = { 421, 112, 100, 10 };
		break;
	default:
		break;
	}


	return rect;
}


void GUISlider::SliderControl(int mouseX, int mouseY)
{

	sliderButton.x = mouseX - (sliderButton.w / 2);

	value = ((maxValue - minValue) * (mouseX - (float)(sliderBar.x + 2 + sliderButton.w / 2))) / (float)(sliderBar.w - 2 - sliderButton.w) + minValue;

	// Limits
	if (sliderButton.x < sliderBar.x + 2)
	{
		sliderButton.x = sliderBar.x + 2;
		value = minValue;
	}
	if ((sliderButton.x + sliderButton.w) > (sliderBar.x + sliderBar.w - 2))
	{
		sliderButton.x = (sliderBar.x + sliderBar.w - 2) - sliderButton.w;
		value = maxValue;
	}

}

void GUISlider::SetValue(int newValue)
{
	this->value = newValue;
	float X, h, h1, h2, h3, i;
	h1 = (((sliderBar.w - sliderButton.w) + minValue) * (maxValue - minValue));
	h2 = maxValue - minValue;
	h3 = (sliderBar.w - sliderButton.w) + minValue;

	h = (h1 * h2) / h3;
	i = (((sliderBar.w - sliderButton.w) + minValue) * (maxValue - minValue)) * value;

	X = (i / h) + (sliderBar.x + (sliderButton.w / 2));
	sliderButton.x = (int)X - 23;
	sliderButton.y = sliderBar.y + (sliderBar.h / 2) - (sliderButton.h / 2);
}