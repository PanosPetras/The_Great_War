#pragma once

#include <istream>

namespace detail {

inline constexpr bool is_ws(int Ch) { // non-locale dependent std::isspace
    return Ch == ' ' || Ch == '\n' || Ch == '\t' || Ch == '\r' || Ch == '\f' || Ch == '\v';
}

template<char Ch>
inline constexpr bool is_ws_v = is_ws(Ch);

template<char... Chars>
inline constexpr bool any_ws_v = (... || is_ws_v<Chars>);

template<bool SkipWhitespace, char... Chars>
struct Eater_impl {
    inline friend std::istream& operator>>(std::istream& is, const Eater_impl&) {
        auto fail = [&](int wanted) {
            //                            non-locale dependent `is >> std::ws`:
            if constexpr(SkipWhitespace)
                while(is_ws(is.peek())) is.ignore();
            if(is.peek() == wanted) {
                is.ignore();
                return false;
            } else {
                return true;
            }
        };
        if((... || fail(Chars))) is.setstate(std::ios::failbit);

        return is;
    }
};

} // namespace detail

template<char... Chars>
struct Eater : detail::Eater_impl<not detail::any_ws_v<Chars...>, Chars...> {};

template<char... Chars>
struct Eater<false, Chars...> : detail::Eater_impl<false, Chars...> {};

template<char... Chars>
struct Eater<true, Chars...> : detail::Eater_impl<true, Chars...> {};
