#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#define MAX_FRAMES 20

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	iPoint offset[MAX_FRAMES];

public:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;

public:
	Animation(){}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	void PushBack(const SDL_Rect& rect, const iPoint& pivot)
	{
		frames[last_frame] = rect;
		offset[last_frame++] = pivot;
	}

	SDL_Rect& GetCurrentFrame()
	{
		return frames[(int)current_frame];
	}

	iPoint& GetCurrentOffset()
	{
		return offset[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
	}
};

#endif
