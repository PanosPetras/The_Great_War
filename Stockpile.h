#ifndef Stockpile_H
#define Stockpile_H

#include <istream>
#include <sstream>
#include <string>

struct Stockpile {
    int Coal = 0;
    int Oil = 0;
    int Timber = 0;
    int Rubber = 0;
    int Cotton = 0;
    int Iron = 0;
    int Grain = 0;
    int Fruit = 0;
    int Electric_gear = 0;
    int Machine_parts = 0;
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

    inline Stockpile& operator+=(const Stockpile& obj) {
        Coal += obj.Coal;
        Oil += obj.Oil;
        Timber += obj.Timber;
        Rubber += obj.Rubber;
        Cotton += obj.Cotton;
        Iron += obj.Iron;
        Grain += obj.Grain;
        Fruit += obj.Fruit;
        Electric_gear += obj.Electric_gear;
        Machine_parts += obj.Machine_parts;
        Glass += obj.Glass;
        Lumber += obj.Lumber;
        Cement += obj.Cement;
        Ammunition += obj.Ammunition;
        Planes += obj.Planes;
        Explosives += obj.Explosives;
        Small_arms += obj.Small_arms;
        Artillery += obj.Artillery;
        Tanks += obj.Tanks;
        Canned_food += obj.Canned_food;
        Furniture += obj.Furniture;
        Clothes += obj.Clothes;
        Automobiles += obj.Automobiles;
        Merchant_ships += obj.Merchant_ships;
        Radios += obj.Radios;
        Telephones += obj.Telephones;
        Fuel += obj.Fuel;
        Paper += obj.Paper;
        Liquor += obj.Liquor;
        Airship += obj.Airship;
        Money += obj.Money;
        return *this;
    }

    inline Stockpile& operator-=(const Stockpile& obj) {
        Coal -= obj.Coal;
        Oil -= obj.Oil;
        Timber -= obj.Timber;
        Rubber -= obj.Rubber;
        Cotton -= obj.Cotton;
        Iron -= obj.Iron;
        Grain -= obj.Grain;
        Fruit -= obj.Fruit;
        Electric_gear -= obj.Electric_gear;
        Machine_parts -= obj.Machine_parts;
        Glass -= obj.Glass;
        Lumber -= obj.Lumber;
        Cement -= obj.Cement;
        Ammunition -= obj.Ammunition;
        Planes -= obj.Planes;
        Explosives -= obj.Explosives;
        Small_arms -= obj.Small_arms;
        Artillery -= obj.Artillery;
        Tanks -= obj.Tanks;
        Canned_food -= obj.Canned_food;
        Furniture -= obj.Furniture;
        Clothes -= obj.Clothes;
        Automobiles -= obj.Automobiles;
        Merchant_ships -= obj.Merchant_ships;
        Radios -= obj.Radios;
        Telephones -= obj.Telephones;
        Fuel -= obj.Fuel;
        Paper -= obj.Paper;
        Liquor -= obj.Liquor;
        Airship -= obj.Airship;
        Money -= obj.Money;
        return *this;
    }

    inline friend std::istream& operator>>(std::istream& is, Stockpile& sp) {
        if(std::string line; std::getline(is, line)) {
            std::istringstream iss(line);
            if(not(iss >> sp.Coal >> sp.Oil >> sp.Timber >> sp.Rubber >> sp.Cotton >> sp.Iron >> sp.Grain >> sp.Fruit >> sp.Electric_gear >> sp.Machine_parts >> sp.Glass >> sp.Lumber >> sp.Cement >> sp.Ammunition >>
                   sp.Planes >> sp.Explosives >> sp.Small_arms >> sp.Artillery >> sp.Tanks >> sp.Canned_food >> sp.Furniture >> sp.Clothes >> sp.Automobiles >> sp.Merchant_ships >> sp.Radios >> sp.Telephones >> sp.Fuel >>
                   sp.Paper >> sp.Liquor >> sp.Airship >> sp.Money)) {
                is.setstate(std::ios::failbit);
            }
        }
        return is;
    }
};

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
