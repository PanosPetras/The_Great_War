#include "Color.h"

#include "Eater.h"

#include <iomanip>
#include <sstream>

std::string Color::toString() const {
    std::ostringstream os;
    os << std::setw(3) << r << '-' << std::setw(3) << g << '-' << std::setw(3) << b;

    return os.str();
}

Color::operator SDL_Color() const noexcept {
    return {r, g, b, 0};
}

inline bool is_fit(int x) {
    return x >= 0 && x <= 0xFF;
}

std::istream& operator>>(std::istream& is, Color& c) {
    Eater<'-'> hyphen;
    if(int R, G, B; is >> R >> hyphen >> G >> hyphen >> B >> Eater<'\n'>{} && is_fit(R) && is_fit(G) && is_fit(B)) {
        c.r = static_cast<Uint8>(R);
        c.g = static_cast<Uint8>(G);
        c.b = static_cast<Uint8>(B);
    } else
        is.setstate(std::ios::failbit);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Color& c) {
    return os << c.toString();
}
