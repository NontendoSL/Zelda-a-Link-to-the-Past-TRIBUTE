#include <math.h>
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Window.h"

j1FadeToBlack::j1FadeToBlack()
{
	name = "fadetoblack";
}

j1FadeToBlack::~j1FadeToBlack()
{}

bool j1FadeToBlack::Awake(pugi::xml_node &conf)
{

	screen = { 0, 0, conf.attribute("width").as_int(), conf.attribute("height").as_int() };

	return true;
}

// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);


	return true;
}

// Update: draw background
bool j1FadeToBlack::Update(float dt)
{
	if (goFade == false)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
			finnishfirstpart = true;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
		{
			current_step = fade_step::none;
			goFade = false;
			finnishfirstpart = false;
		}
	} break;
	}



	return true;
}

bool j1FadeToBlack::IsFading()
{
	return current_step == none;
}

bool j1FadeToBlack::PostUpdate()
{
	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);
	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(float time)
{
	bool ret = false;

	if (goFade == false)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		goFade = true;
		ret = true;
	}

	return ret;
}

bool j1FadeToBlack::Checkfadetoblack()
{
	return finnishfirstpart;
}

bool j1FadeToBlack::Checkfadefromblack()
{
	if (goFade)
	{
		return false;
	}
	else
	{
		finnishfirstpart = false;
		return true;
	}

}
