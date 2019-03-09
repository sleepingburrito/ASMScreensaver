ASMbg or assembly background was a screensaver idea I had. Modern monitors have so many pixels in them that I felt you could show the entire contents of ram on screen at once of an older computer.

So I made a vary simplified 16bit cpu. You program it using its own instruction set and run it to see it work on screen.  Each pixel is bit in memory, white means 1 and black is 0. It is arranged top to bottom, left to right in 16 bit words. 

In the video https://youtu.be/T6Bd42i2JEQ I have two programs shown. The first one just makes up random numbers and writes it to memory filling it up. In the top right side is the CPU registers, under that is the program code itself. Way under that are the random values im writing to memory. The red line shows what current action the CPU did last 

I’m not sure what the next program does but I have an idea of what its trying to test.  It looks like it has two subroutines and its jumping between them to show off the stack. The stack is in the bottom right of the screen.

The whole project is license as MIT.
