#ifndef PERGOOD_H
#define PERGOOD_H

#include <array>
#include <cstddef>
#include <string_view>

/*The one place the goods are enumerated. The Good enum, the members of
PerGood<T> and the table of names further down are all generated from this
list, so none of them can drift out of order, and a new good is added by naming
it here alone.*/
// clang-format off
#define STOCKPILE_GOODS(X)                                               \
    X(Coal) X(Oil) X(Timber) X(Rubber) X(Cotton) X(Iron) X(Copper)       \
    X(Bauxite) X(Nitrates) X(Grain) X(Fruit)                             \
    X(Steel) X(Aluminum) X(Electric_gear) X(Machine_parts) X(Engines)    \
    X(Boilers) X(Glass) X(Lumber) X(Cement) X(Canvas)                    \
    X(Ammunition) X(Planes) X(Explosives) X(Small_arms) X(Artillery)     \
    X(Tanks) X(Canned_food) X(Furniture) X(Clothes) X(Automobiles)       \
    X(Merchant_ships) X(Radios) X(Telephones) X(Fuel) X(Paper) X(Liquor) \
    X(Airship)
// clang-format on

/*Every good the economy trades, in the order a PerGood stores them and a save
file lists them. Money is deliberately not one of them: it is not produced,
consumed or priced the way a good is, so it lives on the Country itself.

The first eleven are what comes out of the ground. States produce only those, so
they lead the list and a state's output can be an array that stops at Fruit.*/
enum class Good {
#define PER_GOOD_ENUMERATOR(name) name,
    STOCKPILE_GOODS(PER_GOOD_ENUMERATOR)
#undef PER_GOOD_ENUMERATOR
};

// Turns a Good into the subscript of the array or table that holds it
inline constexpr std::size_t GoodIndex(Good good) {
    return static_cast<std::size_t>(good);
}

// How many goods there are, which is how wide every per-good table is
inline constexpr std::size_t GoodCount = GoodIndex(Good::Airship) + 1;

// The raw resources, Coal through Fruit, are the leading run of the enum
inline constexpr std::size_t RawGoodCount = GoodIndex(Good::Fruit) + 1;

// Every good, for the code that has to walk them all
inline constexpr std::array<Good, GoodCount> AllGoods{
#define PER_GOOD_VALUE(name) Good::name,
    STOCKPILE_GOODS(PER_GOOD_VALUE)
#undef PER_GOOD_VALUE
};

/*One T for every good. The whole economy is per-good tables of one type or
another - amounts held are ints, prices are doubles, the throughput a shortage
allows is a fraction - so the container is written once and the payload varies.

Because the members are named, a table can still be written out as
PerGood<int>{.Coal = 2, .Iron = 12}, naming only the goods that are not zero.*/
template<typename T>
struct PerGood {
#define PER_GOOD_MEMBER(name) T name{};
    STOCKPILE_GOODS(PER_GOOD_MEMBER)
#undef PER_GOOD_MEMBER

    // The entry for one good, for code that is handed the good to look up
    constexpr T& operator[](Good good);
    constexpr const T& operator[](Good good) const;

    PerGood& operator+=(const PerGood& obj);
    PerGood& operator-=(const PerGood& obj);
    PerGood& operator*=(T factor);
};

/*Each good's member, in the order of the list above. Declared out of line so
that PerGood is a plain aggregate.*/
template<typename T>
inline constexpr std::array<T PerGood<T>::*, GoodCount> GoodMembers{
#define PER_GOOD_POINTER(name) &PerGood<T>::name,
    STOCKPILE_GOODS(PER_GOOD_POINTER)
#undef PER_GOOD_POINTER
};

template<typename T>
inline constexpr T& PerGood<T>::operator[](Good good) {
    return this->*GoodMembers<T>[GoodIndex(good)];
}

template<typename T>
inline constexpr const T& PerGood<T>::operator[](Good good) const {
    return this->*GoodMembers<T>[GoodIndex(good)];
}

template<typename T>
inline PerGood<T>& PerGood<T>::operator+=(const PerGood& obj) {
    for(auto good : GoodMembers<T>) {
        this->*good += obj.*good;
    }
    return *this;
}

template<typename T>
inline PerGood<T>& PerGood<T>::operator-=(const PerGood& obj) {
    for(auto good : GoodMembers<T>) {
        this->*good -= obj.*good;
    }
    return *this;
}

template<typename T>
inline PerGood<T>& PerGood<T>::operator*=(T factor) {
    for(auto good : GoodMembers<T>) {
        this->*good *= factor;
    }
    return *this;
}

template<typename T>
inline PerGood<T> operator+(const PerGood<T>& lhs, const PerGood<T>& rhs) {
    PerGood<T> rv{lhs};
    rv += rhs;
    return rv;
}

template<typename T>
inline PerGood<T> operator-(const PerGood<T>& lhs, const PerGood<T>& rhs) {
    PerGood<T> rv{lhs};
    rv -= rhs;
    return rv;
}

template<typename T>
inline PerGood<T> operator*(const PerGood<T>& lhs, T factor) {
    PerGood<T> rv{lhs};
    rv *= factor;
    return rv;
}

// What a good is for. The Industry screen groups by it, and pop needs read it.
enum class GoodCategory {
    // Comes out of the ground rather than out of a factory
    Raw,

    // Feeds other factories
    Industrial,

    // What the population wants
    Consumer,

    // What an army eats
    Military
};

// Everything about a good that is not an amount of it
struct GoodInfo {
    // The good this row describes. It has to match the row's own position.
    Good good;

    /*The good's name in lower case. It doubles as the name of the icon that
    stands for it under Icons/Goods, so it is singular where the enumerator is
    plural.*/
    std::string_view name;

    GoodCategory category;

    /*What one unit is worth on the world market when supply meets demand,
    which is where prices start and what they drift around. Each is roughly
    what its recipe's inputs and power cost, plus a margin, so a factory adds
    value; raw goods are set against each other by scarcity.*/
    double basePrice;
};

/*One row per good, in the order of the list above. Anything that needs to name
or group a good reads it here rather than spelling the name out again.*/
// clang-format off
inline constexpr std::array<GoodInfo, GoodCount> GoodInfos{{
    {Good::Coal, "coal", GoodCategory::Raw, 2.0},
    {Good::Oil, "oil", GoodCategory::Raw, 4.0},
    {Good::Timber, "timber", GoodCategory::Raw, 1.0},
    {Good::Rubber, "rubber", GoodCategory::Raw, 6.0},
    {Good::Cotton, "cotton", GoodCategory::Raw, 2.0},
    {Good::Iron, "iron", GoodCategory::Raw, 2.0},
    {Good::Copper, "copper", GoodCategory::Raw, 5.0},
    {Good::Bauxite, "bauxite", GoodCategory::Raw, 3.0},
    {Good::Nitrates, "nitrates", GoodCategory::Raw, 4.0},
    {Good::Grain, "grain", GoodCategory::Raw, 1.0},
    {Good::Fruit, "fruit", GoodCategory::Raw, 2.0},
    {Good::Steel, "steel", GoodCategory::Industrial, 6.0},
    {Good::Aluminum, "aluminum", GoodCategory::Industrial, 18.0},
    {Good::Electric_gear, "electric gear", GoodCategory::Industrial, 6.0},
    {Good::Machine_parts, "machine parts", GoodCategory::Industrial, 12.0},
    {Good::Engines, "engine", GoodCategory::Industrial, 45.0},
    {Good::Boilers, "boiler", GoodCategory::Industrial, 70.0},
    {Good::Glass, "glass", GoodCategory::Industrial, 1.5},
    {Good::Lumber, "lumber", GoodCategory::Industrial, 1.5},
    {Good::Cement, "cement", GoodCategory::Industrial, 1.2},
    {Good::Canvas, "canvas", GoodCategory::Industrial, 2.5},
    {Good::Ammunition, "ammunition", GoodCategory::Military, 7.0},
    {Good::Planes, "plane", GoodCategory::Military, 45.0},
    {Good::Explosives, "explosives", GoodCategory::Military, 4.0},
    {Good::Small_arms, "small arms", GoodCategory::Military, 5.0},
    {Good::Artillery, "artillery", GoodCategory::Military, 20.0},
    {Good::Tanks, "tank", GoodCategory::Military, 120.0},
    {Good::Canned_food, "canned food", GoodCategory::Consumer, 3.0},
    {Good::Furniture, "furniture", GoodCategory::Consumer, 4.0},
    {Good::Clothes, "clothes", GoodCategory::Consumer, 3.0},
    {Good::Automobiles, "automobile", GoodCategory::Consumer, 40.0},
    {Good::Merchant_ships, "merchant ship", GoodCategory::Industrial, 16000.0},
    {Good::Radios, "radio", GoodCategory::Consumer, 6.0},
    {Good::Telephones, "telephone", GoodCategory::Consumer, 6.0},
    {Good::Fuel, "fuel", GoodCategory::Industrial, 3.5},
    {Good::Paper, "paper", GoodCategory::Consumer, 1.2},
    {Good::Liquor, "liquor", GoodCategory::Consumer, 2.5},
    {Good::Airship, "airship", GoodCategory::Military, 45000.0},
}};
// clang-format on

/*The table is subscripted by Good, so every row has to sit at its own
enumerator and every enumerator has to have a row.*/
static_assert([] {
    for(std::size_t i = 0; i < GoodInfos.size(); ++i) {
        if(GoodInfos[i].good != Good(i)) return false;
    }
    return true;
}());

// What a good is called and what it is for
inline constexpr const GoodInfo& InfoOf(Good good) {
    return GoodInfos[GoodIndex(good)];
}

#endif
