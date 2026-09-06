#ifndef FONTSIZE_H
#define FONTSIZE_H

#include <algorithm>
#include <array>
#include <cstddef>

/*The set of type sizes the UI is allowed to draw with. A screen names the role
its text plays rather than a point size, so that everything meaning the same
thing stays in step and a size is decided in exactly one place.

The sizes below are authored against a 1080p window and scale with the window's
height. That is the other half of why these are tokens and not constants: a
literal point size keeps the same pixel height at every resolution, so text that
fits its background at 1080p overflows it at 720p.*/
enum class FontSize {
    Title,   // Screen and dialog titles
    Heading, // Section headings and the values a screen is built around - the common case
    Body,    // Running text
    Control, // Button captions
    Caption, // Secondary labels, such as the name beside a flag
    Input,   // Text entry fields
};

// The point size of each token on the 1080p window the UI was laid out against
inline constexpr std::array<int, 6> FontSizeReference{42, 32, 28, 26, 24, 20};

inline constexpr int FontSizeReferenceHeight = 1080;

// Resolves a token to the point size to open the font at, for a window this tall
inline constexpr int Pt(FontSize size, int windowHeight) {
    const int pt = FontSizeReference[static_cast<std::size_t>(size)] * windowHeight / FontSizeReferenceHeight;

    // TTF_OpenFont rejects a size of zero, which a small enough window would produce
    return std::max(pt, 1);
}
#endif
