#pragma once

#include <istream>

struct Coordinate {
    short x, y;

    friend std::istream& operator>>(std::istream&, Coordinate&);
};
