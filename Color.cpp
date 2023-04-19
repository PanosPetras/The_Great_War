#include "Color.h"

#include <iomanip>
#include <sstream>

template<char Ch>
struct Eater {
    friend std::istream& operator>>(std::istream& is, const Eater&) {
        if(is.peek() != Ch) is.setstate(std::ios::failbit);
        else is.ignore();
        return is;
    }
};

std::string Color::toString() const {
    std::ostringstream os;
    os << std::setw(3) << r << '-' <<std::setw(3) << g << '-' << std::setw(3) << b;

    return os.str();
}

Color::operator SDL_Color () const noexcept {
    return {r, g, b, 0};
}


bool is_fit(int x) { return x >= 0 && x <= 0xFF; }

std::istream& operator>>(std::istream& is, Color& c) {
    Eater<'-'> hyphen;
    if(int R, G, B; is >> R >> hyphen >> G >> hyphen >> B >> Eater<'\n'>{} && is_fit(R) && is_fit(G) && is_fit(B) ) {
        c.r = R;
        c.g = G;
        c.b = B;
    } else is.setstate(std::ios::failbit);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Color& c) {
    return os << c.toString();
}
