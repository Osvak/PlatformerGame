#include "AudioManager.h"

#include "App.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"




AudioManager::AudioManager() : Module()
{
	music = NULL;
	name.Create("audioManager");
}

// Destructor
AudioManager::~AudioManager()
{}

// Called before render is available
bool AudioManager::Awake(pugi::xml_node& config)
{
	LOG("AudioManager.Awake(): Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}
	else
	{
		volumeMusic = config.child("music").attribute("volume").as_int(100);
		volumeFX = config.child("fx").attribute("volume").as_int(100);
	}

	// Load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	return ret;
}

// Called before quitting
bool AudioManager::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	ListItem<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.Clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool AudioManager::PlayMusic(const char* path, float fadeTime)
{
	bool ret = true;
	Mix_VolumeMusic(volumeMusic);

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fadeTime > 0.0f)
		{
			Mix_FadeOutMusic(int(fadeTime * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fadeTime > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fadeTime * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}


void AudioManager::StopMusic()
{
	LOG("Stopping music");

	Mix_HaltMusic();
}


// Load WAV
unsigned int AudioManager::LoadFX(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.Add(chunk);
		ret = fx.Count();
		LOG("Loaded FX %s with ID%d", path, ret);
	}

	return ret;
}

bool AudioManager::UnloadFX(unsigned int id)
{
	bool ret = false;
	if (fx[id - 1] != nullptr)
	{
		Mix_FreeChunk(fx[id - 1]);
		fx[id - 1] = nullptr;
		musicList.Del(musicList.At(id));
		LOG("Unloaded FX with ID%d", id);
	}


	return ret;
}


// Play WAV
bool AudioManager::PlayFX(unsigned int id, int volume)
{
	bool ret = false;

	if (volume > 100)
	{
		volume = 100;
	}
	else if (volume < 0)
	{
		volume = 0;
	}

	if (id > 0 && id <= fx.Count())
	{
		if (volume == 0)
		{
			Mix_VolumeChunk(fx[id - 1], volumeFX);
		}
		else
		{
			Mix_VolumeChunk(fx[id - 1], volume);
		}

		Mix_PlayChannel(-1, fx[id - 1], 0);
		ret = true;
	}

	return ret;
}


void AudioManager::SetMusicVolume(int volume)
{
	volumeMusic = volume;
	Mix_VolumeMusic(volumeMusic);
}
void AudioManager::SetFXVolume(int volume)
{
	volumeFX = volume;
}
int AudioManager::GetMusicVolume()
{
	return (int)volumeMusic;
}
int AudioManager::GetFXVolume()
{
	return (int)volumeFX;
}


bool AudioManager::LoadState(pugi::xml_node& node)
{
	volumeMusic = node.child("music").attribute("volume").as_int();
	volumeFX = node.child("fx").attribute("volume").as_int();
	Mix_VolumeMusic(volumeMusic);


	return true;
}
bool AudioManager::SaveState(pugi::xml_node& node) const
{
	pugi::xml_node audio = node.append_child("audioManager");
	audio.child("music").attribute("volume").set_value(volumeMusic);
	audio.child("fx").attribute("volume").set_value(volumeFX);


	return true;
}