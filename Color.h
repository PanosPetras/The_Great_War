#pragma once
#include <string>

//8 bit unsigned integer
typedef unsigned char uint8;

typedef struct Color {
    uint8 r;
    uint8 g;
    uint8 b;

    std::string toString();
} Color;