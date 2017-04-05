#pragma once
#ifndef __j1FADETOBLACK_H__
#define __j1FADETOBLACK_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();
	~j1FadeToBlack();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	// Call modules after each loop iteration
	bool PostUpdate();

	bool FadeToBlack(float time = 2.0f);

	bool Checkfadetoblack();
	bool Checkfadefromblack();

	bool IsFading();

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	};

	fade_step current_step = none;
private:

	float normalized = 0;

	bool goFade = false;
	bool finnishfirstpart = false;
	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
};

#endif //__j1FADETOBLACK_H__
