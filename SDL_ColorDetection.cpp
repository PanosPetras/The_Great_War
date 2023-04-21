#include "SDL_ColorDetection.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace CD {

Uint32 getpixel(SDL_Surface_ctx& surface, int x, int y) {
    static_assert(SDL_BYTEORDER == SDL_LIL_ENDIAN || SDL_BYTEORDER == SDL_BIG_ENDIAN, "Unsupported endianess");
    const SDL_PixelFormat& fmt = *surface->format;
    // Get the bit depth of the surface
    int bpp = fmt.BytesPerPixel;

    /* Here p is the address to the pixel we want to retrieve */
    auto p = static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * bpp;

    std::unique_ptr<SDL_Surface, decltype([](SDL_Surface* s) { ::SDL_UnlockSurface(s); })> unlocker(surface);
    ::SDL_LockSurface(surface); // Cheap if locking isn't really required

    // We extract the color.
    // The way that we extract it depends on the bit depth of the surface
    // Note that the map of the game has 3 bytes per pixel
    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *reinterpret_cast<Uint16*>(p);

    case 3:
        if constexpr(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
            return p[0] | Uint32(p[1]) << 8 | Uint32(p[2]) << 16;
        } else {
            return Uint32(p[0]) << 16 | Uint32(p[1]) << 8 | p[2];
        }

    case 4:
        return *reinterpret_cast<Uint32*>(p);

    default:
        // shouldn't happen, but avoids warnings:
        throw std::runtime_error("CD::getpixel BPP: " + std::to_string(bpp));
    }
}

SDL_Color getcolor(SDL_Surface_ctx& surface, int x, int y) {
    SDL_Color rgb;

    // Get the requested pixel's data
    Uint32 pixel = getpixel(surface, x, y);

    // Extract the color
    ::SDL_GetRGBA(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b, &rgb.a);

    return rgb;
}

} // namespace CD
