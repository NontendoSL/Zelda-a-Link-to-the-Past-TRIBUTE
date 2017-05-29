// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1TimeNormal.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1TimeNormal::j1TimeNormal()
{
	Start();
}

// ---------------------------------------------
void j1TimeNormal::Start()
{
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
uint32 j1TimeNormal::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float j1TimeNormal::ReadSec()
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}