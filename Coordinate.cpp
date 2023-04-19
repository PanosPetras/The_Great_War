#include "Coordinate.h"

#include "Eater.h"

#include <sstream>
#include <string>
#include <utility>

std::istream& operator>>(std::istream& is, Coordinate& c) {
    if(std::string line; std::getline(is, line)) {
        if(line == "---") {
            c.x = -1;
            c.y = -1;
        } else {
            std::istringstream iss(std::move(line));
            if(not(iss >> c.x >> Eater<'-','-','-'>{} >> c.y)) {
                is.setstate(std::ios::failbit);
            }
        }
    } else is.setstate(std::ios::failbit);
    return is;
}
