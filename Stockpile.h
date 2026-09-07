#ifndef Stockpile_H
#define Stockpile_H

#include <array>
#include <cstddef>
#include <istream>
#include <sstream>
#include <string>

/*The one place the goods are enumerated. Both the Good enum and the table of
members further down are generated from this list, so the two cannot drift out
of order, and a new good is added by naming it here and declaring the field it
lives in. A name that matches no field fails to compile.*/
// clang-format off
#define STOCKPILE_GOODS(X)                                               \
    X(Coal) X(Oil) X(Timber) X(Rubber) X(Cotton) X(Iron) X(Copper)       \
    X(Nitrates) X(Grain) X(Fruit)                                        \
    X(Steel) X(Aluminum) X(Electric_gear) X(Machine_parts) X(Engines)    \
    X(Boilers) X(Glass) X(Lumber) X(Cement)                              \
    X(Ammunition) X(Planes) X(Explosives) X(Small_arms) X(Artillery)     \
    X(Tanks) X(Canned_food) X(Furniture) X(Clothes) X(Automobiles)       \
    X(Merchant_ships) X(Radios) X(Telephones) X(Fuel) X(Paper) X(Liquor) \
    X(Airship)
// clang-format on

/*Every good the economy trades, in the order a Stockpile stores them and a save
file lists them. Money is deliberately not one of them: it is not produced,
consumed or priced the way a good is, so it stays a field of its own.

The first ten are what comes out of the ground. States produce only those, so
they lead the list and a state's output can be an array that stops at Fruit.*/
enum class Good {
#define STOCKPILE_GOOD_ENUMERATOR(name) name,
    STOCKPILE_GOODS(STOCKPILE_GOOD_ENUMERATOR)
#undef STOCKPILE_GOOD_ENUMERATOR
};

// Turns a Good into the subscript of the array or table that holds it
inline constexpr std::size_t GoodIndex(Good good) {
    return static_cast<std::size_t>(good);
}

// The raw resources, Coal through Fruit, are the leading run of the enum
inline constexpr std::size_t RawGoodCount = GoodIndex(Good::Fruit) + 1;

struct Stockpile {
    int Coal = 0;
    int Oil = 0;
    int Timber = 0;
    int Rubber = 0;
    int Cotton = 0;
    int Iron = 0;
    int Copper = 0;
    int Nitrates = 0;
    int Grain = 0;
    int Fruit = 0;
    int Steel = 0;
    int Aluminum = 0;
    int Electric_gear = 0;
    int Machine_parts = 0;
    int Engines = 0;
    int Boilers = 0;
    int Glass = 0;
    int Lumber = 0;
    int Cement = 0;
    int Ammunition = 0;
    int Planes = 0;
    int Explosives = 0;
    int Small_arms = 0;
    int Artillery = 0;
    int Tanks = 0;
    int Canned_food = 0;
    int Furniture = 0;
    int Clothes = 0;
    int Automobiles = 0;
    int Merchant_ships = 0;
    int Radios = 0;
    int Telephones = 0;
    int Fuel = 0;
    int Paper = 0;
    int Liquor = 0;
    int Airship = 0;
    long long Money = 0;

    // The amount held of one good, for code that is handed the good to look up
    int& operator[](Good good);
    int operator[](Good good) const;

    Stockpile& operator+=(const Stockpile& obj);
    Stockpile& operator-=(const Stockpile& obj);
};

// Each good's field, in the order of the list above, for the code that walks them all
inline constexpr std::array Goods{
#define STOCKPILE_GOOD_MEMBER(name) &Stockpile::name,
    STOCKPILE_GOODS(STOCKPILE_GOOD_MEMBER)
#undef STOCKPILE_GOOD_MEMBER
};

inline int& Stockpile::operator[](Good good) {
    return this->*Goods[GoodIndex(good)];
}

inline int Stockpile::operator[](Good good) const {
    return this->*Goods[GoodIndex(good)];
}

inline Stockpile& Stockpile::operator+=(const Stockpile& obj) {
    for(auto good : Goods) {
        this->*good += obj.*good;
    }
    Money += obj.Money;
    return *this;
}

inline Stockpile& Stockpile::operator-=(const Stockpile& obj) {
    for(auto good : Goods) {
        this->*good -= obj.*good;
    }
    Money -= obj.Money;
    return *this;
}

/*One stockpile per line: the goods in the order of the table above, then the
money. A failed extraction leaves the stream in a failed state, so only the
last one needs checking.*/
inline std::istream& operator>>(std::istream& is, Stockpile& sp) {
    if(std::string line; std::getline(is, line)) {
        std::istringstream iss(line);
        for(auto good : Goods) {
            iss >> sp.*good;
        }
        if(not(iss >> sp.Money)) {
            is.setstate(std::ios::failbit);
        }
    }
    return is;
}

inline Stockpile operator+(const Stockpile& lhs, const Stockpile& rhs) {
    Stockpile rv{lhs};
    rv += rhs;
    return rv;
}

inline Stockpile operator-(const Stockpile& lhs, const Stockpile& rhs) {
    Stockpile rv{lhs};
    rv -= rhs;
    return rv;
}

#endif
