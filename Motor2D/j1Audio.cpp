#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name="audio";
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	//p2List_item<Mix_Chunk*>* item; TODO
	std::list<Mix_Chunk*>::iterator item;
	for(item = fx.begin(); item != fx.end(); item++)
		Mix_FreeChunk(item._Ptr->_Myval);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS_RW(App->fs->Load(path), 1);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
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


bool j1Audio::StopMusic()
{
	bool ret = true;
	if (music != NULL)
	{
		Mix_PauseMusic();
		if (Mix_PausedMusic == 0)
		{
			LOG("Music wasn't paused. Mix_GetError(): %s", Mix_GetError());
			ret = false;
		}
		else
		{
			LOG("Music Paused");
		}
	}
	return ret;
}

bool j1Audio::ResumeMusic()
{
	bool ret = true;
	if (music != NULL)
	{
		Mix_ResumeMusic();
		if (Mix_Playing(-1) == 0)
		{
			LOG("Music is now playing.");
			ret = false;
		}
		else
		{
			LOG("Music dosent play. Mix_GetError(): %s", Mix_GetError());
		}
	}
	return ret;
}

void j1Audio::VolumeMusic(int volume)
{
	if (music != NULL)
	{
		LOG("volume was    : %d\n", Mix_VolumeMusic(MIX_MAX_VOLUME / 2));
		Mix_VolumeMusic(volume);
		LOG("volume is now : %d\n", Mix_VolumeMusic(-1));
	}
}

void j1Audio::FadeMusic(int ms)
{
	if (music != NULL)
	{
		Mix_FadeOutMusic((int)(ms * 1000.0f));
	}
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV_RW(App->fs->Load(path), 1);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.size())
	{
		std::list<Mix_Chunk*>::iterator item = fx.begin();
		int i = 0;
		while (i < id - 1)
		{
			item++;
			i++;
		}
		Mix_PlayChannel(-1, item._Ptr->_Myval, repeat);
	}

	return ret;
}