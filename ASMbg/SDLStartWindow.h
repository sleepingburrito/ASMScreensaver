#pragma once
#include <SDL.h>
#include <stdbool.h>

//public, readonly
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Uint8 *keyState = NULL;


//false = failed
//run this in SDL's main
bool StartWindow(int w, int h)
{
	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		if ( 
				  ( (window   = SDL_CreateWindow( "", 0, 0, w, h, 0)) != NULL )
				&&  (renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ))
		   )
		{
			
			keyState = (Uint8*)SDL_GetKeyboardState(NULL);
			return true;
		}
	}

	return false;
}