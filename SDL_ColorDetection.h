#ifndef SDL_COLORDETECTION_CPP
#define SDL_COLORDETECTION_CPP

#include <SDL.h>
#include <iostream>

//Color Detection(CD) namespace
namespace CD {
	//Return the selected pixel's data
	Uint32 getpixel(SDL_Surface* surface, int x, int y);

	//Extract the pixel's color from it's data
	SDL_Color getcolor(SDL_Surface* surface, int x, int y);
}

#endif