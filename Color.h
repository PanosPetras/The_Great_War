#pragma once

#include <SDL.h>

#include <istream>
#include <ostream>
#include <string>

struct Color {
    constexpr Color() = default;
    inline constexpr Color(Uint8 R, Uint8 G, Uint8 B) : r{R}, g{G}, b{B} {}
    inline explicit constexpr Color(const SDL_Color sc) : r{sc.r}, g{sc.g}, b{sc.b} {}

    std::string toString() const;

    explicit operator SDL_Color() const noexcept;

    friend std::istream& operator>>(std::istream& is, Color& c);
    friend std::ostream& operator<<(std::ostream& os, const Color& c);

    Uint8 r{};
    Uint8 g{};
    Uint8 b{};
};
