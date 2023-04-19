#pragma once

template <char... Chars>
struct Eater {
    inline friend std::istream& operator>>(std::istream& is, const Eater&) {
        auto fail = [&](int wanted) {
            if (is.peek() == wanted) {
                is.ignore();
                return false;
            } else {
                return true;
            }
        };
        if ((... || fail(Chars))) is.setstate(std::ios::failbit);
        return is;
    }
};
