#pragma once
#include <string>

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;

    std::string toString();
} Color;