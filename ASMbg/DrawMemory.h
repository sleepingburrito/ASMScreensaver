#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include "SDLStartWindow.h"
#include "Memory.h"

typedef struct
{
	//only draw what has changed
	uint16_t * deltaMainMemory;

	//colors
	unsigned char backgroundR, backgroundG, backgroundB;
	unsigned char programCounterR, programCounterG, programCounterB;
	unsigned char bitR, bitG, bitB;

	//height of screen
	size_t height;

	//draw step
	size_t step; //internal use
	size_t stepMax;
} DrawingSettings;


//memory management
bool InitializedDrawSettings
(
	DrawingSettings * inputDrawSettings,
	unsigned char backgroundR,     unsigned char backgroundG,     unsigned char backgroundB,
	unsigned char programCounterR, unsigned char programCounterG, unsigned char programCounterB,
	unsigned char bitR,            unsigned char bitG,            unsigned char bitB,
	size_t stepMax, size_t height
)
{
	if ( inputDrawSettings == NULL )
		return false;

	if ( ( inputDrawSettings->deltaMainMemory = InitializedMainMemory() ) == NULL )
		return false;

	inputDrawSettings->step = 0;
	inputDrawSettings->stepMax = stepMax;

	inputDrawSettings->backgroundR = backgroundR;
	inputDrawSettings->backgroundG = backgroundG;
	inputDrawSettings->backgroundB = backgroundB;

	inputDrawSettings->programCounterR = programCounterR;
	inputDrawSettings->programCounterG = programCounterG;
	inputDrawSettings->programCounterB = programCounterB;

	inputDrawSettings->bitR = bitR;
	inputDrawSettings->bitG = bitG;
	inputDrawSettings->bitB = bitB;

	inputDrawSettings->height = height;
}

void FreeDrawSettings( DrawingSettings * inputDrawSettings )
{
	if ( inputDrawSettings == NULL )
		return;

	free( inputDrawSettings->deltaMainMemory );
}


//drawing tools
void DrawClearScreen( DrawingSettings * inputDrawSettings, SDL_Renderer * screen )
{
	if ( inputDrawSettings == NULL )
		return;

	//reset memory delta to starting values
	memset( inputDrawSettings, 0x0, RAM_CAPACITY );

	//clear the screen
	SDL_SetRenderDrawColor( screen, inputDrawSettings->backgroundR, inputDrawSettings->backgroundG, inputDrawSettings->backgroundB, 0xFF );
	SDL_RenderClear( screen  );
}

void DrawMemory( const uint16_t * memeory, DrawingSettings * inputDrawSettings, SDL_Renderer * screen )
{
	//check draw step to see if we can draw
	if ( memeory == NULL && inputDrawSettings == NULL && inputDrawSettings->step++ < inputDrawSettings->stepMax )
		return;

	//reset draw step
	inputDrawSettings->step = 0;

	//starting pixel of eatch word
	size_t x = 0;
	size_t y = 0;

	for( size_t wordIndex = 0; wordIndex < RAM_CAPACITY; wordIndex++ )
	{
		uint16_t tmpWord = memeory[wordIndex];

		if ( tmpWord != inputDrawSettings->deltaMainMemory[wordIndex] || wordIndex == ProgramCounterValue( memeory ) )
		{
			for( size_t bitIndex = 0; bitIndex < BITS_PER_WORD; bitIndex++, tmpWord <<= 1 )
				if ( ( tmpWord & 0x8000 ) != 0 ) //if bit at this word value is on
				{
					SDL_SetRenderDrawColor( renderer, inputDrawSettings->bitR, inputDrawSettings->bitG, inputDrawSettings->bitB, 255 );
					SDL_RenderDrawPoint( renderer, (int)( x + bitIndex ), (int)y );
				}
				else if ( wordIndex == ProgramCounterValue( memeory ) ) //if pixel is background and is on program counter
				{
					SDL_SetRenderDrawColor( renderer, inputDrawSettings->programCounterR, inputDrawSettings->programCounterG, inputDrawSettings->programCounterB, 255 );
					SDL_RenderDrawPoint( renderer, (int)( x + bitIndex ), (int)y );
				}
				else //normal black background default
				{
					SDL_SetRenderDrawColor( renderer, inputDrawSettings->backgroundR, inputDrawSettings->backgroundG, inputDrawSettings->backgroundB, 255 );
					SDL_RenderDrawPoint( renderer, (int)( x + bitIndex ), (int)y );
				}

			inputDrawSettings->deltaMainMemory[wordIndex] = memeory[wordIndex]; //update delta table to new value
		}

		//move drawing to next free space
		if ( ++y >= inputDrawSettings->height )
			y = 0, x += BITS_PER_WORD;
	}

	//reset this value to get redrawn next time to remove program counter background
	inputDrawSettings->deltaMainMemory[ProgramCounterValue( memeory )] = inputDrawSettings->deltaMainMemory[ProgramCounterValue( memeory )] + 1;
}