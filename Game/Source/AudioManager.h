#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include "Module.h"

#include "List.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f



struct _Mix_Music;
struct Mix_Chunk;

class AudioManager : public Module
{
public:

	AudioManager();

	// Destructor
	virtual ~AudioManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);
	void StopMusic();

	// Load a WAV in memory
	unsigned int LoadFX(const char* path);
	bool UnloadFX(unsigned int id);

	// Play a previously loaded WAV
	bool PlayFX(unsigned int fx, int repeat = 0);
	
public:

	List<unsigned int*> musicList;

private:

	_Mix_Music* music;
	List<Mix_Chunk *>	fx;
};

#endif // __AUDIOMANAGER_H__