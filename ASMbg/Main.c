#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "SDLtimingTools.h"
#include "SDLStartWindow.h"
#include "SDLkeyIO.h"
#include "asmbg.h"
#include "Setting.h"


int main(int argc, char* argv[])
{
	//perfrmace satisitcs timer
	Uint32 updateDisplayPref = TimerSet(1000);
	StartWindow(WINDOW_WIDTH, WINDOW_HIGHT);

	//testing
	Start();

	int drawStep = 0; //how many loops till you draw something
	int drawStepMax = 0;

	//main loop
	SDL_Event event;
	while (true)
	{
		//io events
		SDL_PollEvent(&event);
		if( event.type == SDL_QUIT || keyState[SDL_SCANCODE_ESCAPE] )
			return 0;


		//
		//logic
		if (keyState[SDL_SCANCODE_SPACE])
			TickComputer(), SDL_Delay(500);

		TickComputer();

		//
		//drawing
		if( drawStep++ > drawStepMax )
		{
			drawStep = 0;

			DrawMemory();

			SDL_RenderPresent(renderer);
		}

		//perframcing mangament
		UpdateAllTimeTools();
		if (TimerPassed(updateDisplayPref))
		{
			printf("deltaTime: %f fps: %f\n", deltaTime, 1000.0f / deltaTime);
			updateDisplayPref = TimerSet(1000);
		}
		
	}

	SDL_Quit();
	return 0;
}