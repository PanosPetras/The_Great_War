#ifndef SDL_COLORDETECTION_CPP
#define SDL_COLORDETECTION_CPP

#include "Color.h"

#include "SDL_ctx.h"

// Color Detection(CD) namespace
namespace CD {
// Return the selected pixel's data
Uint32 getpixel(SDL_Surface_ctx& surface, int x, int y);

// Extract the pixel's color from it's data
SDL_Color getcolor(SDL_Surface_ctx& surface, int x, int y);
} // namespace CD

#endif
