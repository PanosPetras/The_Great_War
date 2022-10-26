#include "SDL_ColorDetection.h"

Uint32 CD::getpixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	//Note that the map has 24 bits per pixel
	return p[0] | p[1] << 8 | p[2] << 16;

	/* Example working for multiple bitdepths
	kept only for archive purposes, as of now it's useless
	switch (bpp)
	{
	case 1:
		return *p;
		break;
		case 2:
		return *(Uint16*)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32*)p;
		break;

	default:
		return 0;       // shouldn't happen, but avoids warnings
	}*/
}

SDL_Color CD::getcolor(SDL_Surface* surface, int x, int y) {
	SDL_Color rgb;

	//Get the requested pixel's data
	Uint32 data = CD::getpixel(surface, x, y);

	//Extract the color
	SDL_GetRGB(data, surface->format, &rgb.r, &rgb.g, &rgb.b);

	return rgb;
}