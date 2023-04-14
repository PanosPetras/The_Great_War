#include "SDL_ColorDetection.h"

Uint32 CD::getpixel(SDL_Surface_ctx& surface, int x, int y){
	//Get the bit depth of the surface
	int bpp = surface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to retrieve */
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	//We extract the color.
	//The way that we extract it depends on the bit depth of the surface
	//Note that the map of the game has 3 bytes per pixel
	switch (bpp)
	{
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16*)p;
		break;

	case 3:
		return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32*)p;
		break;

	default:
		return 0;       // shouldn't happen, but avoids warnings
	}
}

Color CD::getcolor(SDL_Surface_ctx& surface, int x, int y) {
	Color rgb;

	//Get the requested pixel's data
	Uint32 data = CD::getpixel(surface, x, y);

	//Extract the color
	SDL_GetRGB(data, surface->format, &rgb.r, &rgb.g, &rgb.b);

	return rgb;
}
