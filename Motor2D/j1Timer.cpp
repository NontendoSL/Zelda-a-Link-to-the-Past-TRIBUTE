// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"
#include "j1App.h"
#include "j1Scene.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();
}

// ---------------------------------------------
void j1Timer::Start()
{
	started_at = SDL_GetTicks();
	stop = false;
	notsumagain = false;
}

// ---------------------------------------------
uint32 j1Timer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float j1Timer::ReadSec()
{
	if (App->scene->stop && stop == false)
	{
		stop = true;
	}

	if (stop)
	{
		if (notsumagain == false && App->scene->gamestate != INMENU)
		{
			started_at += App->scene->timepaused32;
			notsumagain = true;
		}
		return float(SDL_GetTicks() - started_at) / 1000.0f;
	}
	else
	{
		return float(SDL_GetTicks() - started_at) / 1000.0f;
	}
}