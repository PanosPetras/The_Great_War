#include "Coordinate.h"

#include <string>

std::istream& operator>>(std::istream& is, Coordinate& c) {
    if(std::string line; std::getline(is, line)) {
        if(line == "---") {
            c.x = -1;
            c.y = -1;
        } else if(line.size() < 11) {
            is.setstate(std::ios::failbit);
        } else {
            c.x = std::stoi(line.substr(0, 4));
            c.y = std::stoi(line.substr(7, 4));
        }
    } else is.setstate(std::ios::failbit);
    return is;
}
