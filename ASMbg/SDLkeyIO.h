#pragma once
#include "SDLStartWindow.h"
#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>

//public
enum gameKeys
{
	KeyRotateLeft,
	KeyRotateRight,
	KeyRotateAtAll,
	KeyLeft,
	KeyRight,
	KeySoftDown,
	KeyHardDOwn,
	KeyMenu,
	KeyCount
};

enum gameKeyStates
{
	KeyStateDown,
	KeyStateTap, //only true for the frame it happens
	KeyStateCount
};

bool keyIOStates[KeyCount * KeyStateCount]; //key states external access, read only
char KeyMappings[KeyCount] = //This is internal. It goes in order of gameKeys, they should have the same count
{
	SDL_SCANCODE_Q,
	SDL_SCANCODE_E,
	SDL_SCANCODE_W,
	SDL_SCANCODE_A,
	SDL_SCANCODE_D,
	SDL_SCANCODE_S,
	SDL_SCANCODE_SPACE,
	SDL_SCANCODE_RETURN
};

void UpdateKeys()
{
	for (char i = 0; i < KeyCount; i++)
	{
		keyIOStates[i * KeyStateCount] = keyState[KeyMappings[i]];

		if (keyState[KeyMappings[i]])
		{
			if (!keyIOStates[i * KeyStateCount])//KeyStateTap only works for the first time the key is tapped
				keyIOStates[i * KeyStateCount + 1] = true;
			else
				keyIOStates[i * KeyStateCount + 1] = false;

			keyIOStates[i * KeyStateCount] = true;
		}
		else
		{
			keyIOStates[i * KeyStateCount] = false;
			keyIOStates[i * KeyStateCount + 1] = false; //i * KeyStateCount + 1 is to set KeyStateTap
		}
	}
}

void LoadKeyMappingFile(const char * filename)
{

}

//used to save the default keys
void SaveKeyMappingFile(const char * filename)
{
	FILE * file = fopen(filename, "w");

	if (file == NULL)
	{
		SDL_Log("SaveKeyMappingFile failed to open file: %s", filename);
		return;
	}

	for (char i = 0; i < KeyCount; i++)
		fprintf(file, "%s\n", SDL_GetScancodeName((SDL_Scancode)KeyMappings[i]));

	fclose(file);
}