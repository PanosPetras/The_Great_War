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
};

/*One row per good, in the order of the list above. Anything that needs to name
or group a good reads it here rather than spelling the name out again.*/
// clang-format off
inline constexpr std::array<GoodInfo, GoodCount> GoodInfos{{
    {Good::Coal, "coal", GoodCategory::Raw},
    {Good::Oil, "oil", GoodCategory::Raw},
    {Good::Timber, "timber", GoodCategory::Raw},
    {Good::Rubber, "rubber", GoodCategory::Raw},
    {Good::Cotton, "cotton", GoodCategory::Raw},
    {Good::Iron, "iron", GoodCategory::Raw},
    {Good::Copper, "copper", GoodCategory::Raw},
    {Good::Bauxite, "bauxite", GoodCategory::Raw},
    {Good::Nitrates, "nitrates", GoodCategory::Raw},
    {Good::Grain, "grain", GoodCategory::Raw},
    {Good::Fruit, "fruit", GoodCategory::Raw},
    {Good::Steel, "steel", GoodCategory::Industrial},
    {Good::Aluminum, "aluminum", GoodCategory::Industrial},
    {Good::Electric_gear, "electric gear", GoodCategory::Industrial},
    {Good::Machine_parts, "machine parts", GoodCategory::Industrial},
    {Good::Engines, "engine", GoodCategory::Industrial},
    {Good::Boilers, "boiler", GoodCategory::Industrial},
    {Good::Glass, "glass", GoodCategory::Industrial},
    {Good::Lumber, "lumber", GoodCategory::Industrial},
    {Good::Cement, "cement", GoodCategory::Industrial},
    {Good::Canvas, "canvas", GoodCategory::Industrial},
    {Good::Ammunition, "ammunition", GoodCategory::Military},
    {Good::Planes, "plane", GoodCategory::Military},
    {Good::Explosives, "explosives", GoodCategory::Military},
    {Good::Small_arms, "small arms", GoodCategory::Military},
    {Good::Artillery, "artillery", GoodCategory::Military},
    {Good::Tanks, "tank", GoodCategory::Military},
    {Good::Canned_food, "canned food", GoodCategory::Consumer},
    {Good::Furniture, "furniture", GoodCategory::Consumer},
    {Good::Clothes, "clothes", GoodCategory::Consumer},
    {Good::Automobiles, "automobile", GoodCategory::Consumer},
    {Good::Merchant_ships, "merchant ship", GoodCategory::Industrial},
    {Good::Radios, "radio", GoodCategory::Consumer},
    {Good::Telephones, "telephone", GoodCategory::Consumer},
    {Good::Fuel, "fuel", GoodCategory::Industrial},
    {Good::Paper, "paper", GoodCategory::Consumer},
    {Good::Liquor, "liquor", GoodCategory::Consumer},
    {Good::Airship, "airship", GoodCategory::Military},
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
