#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "j1Module.h"
#include "j1PerfTimer.h"

enum THEORAPLAY_VideoFormat;
struct THEORAPLAY_AudioPacket;
struct THEORAPLAY_Decoder;
struct THEORAPLAY_VideoFrame;

struct AudioQueue 
{
	const THEORAPLAY_AudioPacket *audio;
	int offset;
	struct AudioQueue *next;
};

class Video : public j1Module
{
public:

	Video();

	// Destructor
	virtual ~Video();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Update videoframes and audiopackets
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Play a video file
	void PlayVideo(const char *fname, SDL_Rect r);

	// Audio methods
	static void SDLCALL audio_callback(void *userdata, Uint8 *stream, int len);
	static void queue_audio(const THEORAPLAY_AudioPacket *audio);

private:
	void ResetValues();

	// Load video file
	void LoadVideo(const char *fname);

private:
	THEORAPLAY_Decoder* decoder;
	const THEORAPLAY_VideoFrame* video;
	const THEORAPLAY_AudioPacket* audio;
	SDL_Window* screen;
	SDL_Texture* texture;
	SDL_AudioSpec spec;
	SDL_Event event;

	Uint32 baseticks;
	Uint32 framems;
	int init_failed;
	int quit;

	void* pixels;
	int pitch;
	bool want_to_play;

	static AudioQueue* audio_queue;
	static AudioQueue* audio_queue_tail;

	SDL_Rect rendering_rect;
};

#endif // __VIDEO_H__