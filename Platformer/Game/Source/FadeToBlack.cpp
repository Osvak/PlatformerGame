#include "FadeToBlack.h"

#include "App.h"
#include "Render.h"
#include "Window.h"


FadeToBlack::FadeToBlack() : Module()
{
	name.Create("fade");
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen -----");
	screenRect = { 0, 0, app->win->screenSurface->w, app->win->screenSurface->h };

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE)
	{
		return true;
	}

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			currentStep = Fade_Step::FROM_BLACK;
			if (moduleToDisable->active == true)
			{
				moduleToDisable->CleanUp();
			}
			if (moduleToEnable->active == false)
			{
				moduleToEnable->Init();
				moduleToEnable->Start();
			}
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}

	return true;
}

bool FadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool FadeToBlack::Fade(Module* toDisable, Module* toEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		moduleToDisable = toDisable;
		moduleToEnable = toEnable;

		ret = true;
	}

	return ret;
}