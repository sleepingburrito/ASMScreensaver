#pragma once
#include <SDL.h>
#include <stdbool.h>

//pubic members
float deltaTime = 0; //is in milliseconds

//needs to be called eatch frame to be updated
void UpdateDeltaTime()
{
	static int timeLastFrame = 0;

	deltaTime = (float)(SDL_GetTicks() - timeLastFrame);
	timeLastFrame = SDL_GetTicks();
}


//timer tools
Uint32 TimerSet(int time)
{
	return SDL_GetTicks() + time;
}

bool TimerPassed(Uint32 timer)
{
	return timer < SDL_GetTicks();
}


//updating time tools
void UpdateAllTimeTools()
{
	UpdateDeltaTime();
}