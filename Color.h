#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

struct Color {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;

    std::string toString() const;

    friend std::istream& operator>>(std::istream& is, Color& c);
    friend std::ostream& operator<<(std::ostream& os, const Color& c);
};
