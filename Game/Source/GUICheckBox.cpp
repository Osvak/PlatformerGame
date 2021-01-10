#include "GUICheckBox.h"



GUICheckBox::GUICheckBox(uint32 id, SDL_Rect bounds, const char* text, AudioManager* audioManager, Fonts* fonts, bool initState) : GUIControl(GUIControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->texture = texture;
	this->text = text;
	this->audioManager = audioManager;
	this->fonts = fonts;


	buttonFocusedFX = audioManager->LoadFX("Assets/Audio/FX/button_focused.wav");
	audioManager->musicList.Add(&buttonFocusedFX);
	buttonPressedFX = audioManager->LoadFX("Assets/Audio/FX/button_pressed.wav");
	audioManager->musicList.Add(&buttonPressedFX);

	guiFont = fonts->Load("Assets/Fonts/font_white.png", "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ", 2, 189, 18);

	checked = initState;
}

GUICheckBox::~GUICheckBox()
{
	audioManager->UnloadFX(buttonFocusedFX);
	audioManager->UnloadFX(buttonPressedFX);
	fonts->UnLoad(guiFont);
}

bool GUICheckBox::Update(Input* input, float dt)
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
				checked = !checked;
				if (checked == true)
				{
					value = 1;
				}
				else
				{
					value = 0;
				}
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

bool GUICheckBox::Draw(Render* render, bool drawGUI)
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
			render->DrawRectangle(bounds, 100, 100, 100, 150);
		}


		break;
	}

	case GUIControlState::FOCUSED:
	{
		render->DrawTexture(texture, bounds.x, bounds.y, &rect);

		if (drawGUI == true)
		{
			render->DrawRectangle(bounds, 100, 100, 100, 150);
		}


		break;
	}

	case GUIControlState::PRESSED:
	{
		render->DrawTexture(texture, bounds.x, bounds.y, &rect);

		if (drawGUI == true)
		{
			render->DrawRectangle(bounds, 100, 100, 100, 150);
		}


		break;
	}

	default:
		break;
	}

	int posX = bounds.x + ((int)((float)bounds.w / 2));
	posX = posX - (int)((((float)text.Length() / 2) + 0.5f) * 8);
	int posY = bounds.y - 10;
	fonts->DrawText(posX, posY, guiFont, text.GetString());


	return false;
}

SDL_Rect GUICheckBox::FindPositionInAtlas()
{
	SDL_Rect rect = { 0, 0, 0, 0 };

	switch (state)
	{
	case GUIControlState::DISABLED:
	{
		if (checked == true)
		{
			rect = { 322, 150, 15, 15 };
		}
		else
		{
			rect = { 217, 150, 15, 15 };
		}


		break;
	}

	case GUIControlState::NORMAL:
	{
		if (checked == true)
		{
			rect = { 322, 32, 15, 15 };
		}
		else
		{
			rect = { 217, 32, 15, 15 };
		}


		break;
	}

	case GUIControlState::FOCUSED:
	{
		if (checked == true)
		{
			rect = { 322, 71, 15, 15 };
		}
		else
		{
			rect = { 217, 71, 15, 15 };
		}


		break;
	}

	case GUIControlState::PRESSED:
	{
		if (checked == true)
		{
			rect = { 322, 110, 15, 15 };
		}
		else
		{
			rect = { 217, 110, 15, 15 };
		}


		break;
	}

	default:
		break;
	}


	return rect;
}