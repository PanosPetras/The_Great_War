#ifndef Stockpile_H
#define Stockpile_H

typedef struct Stockpile {
public:
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

    struct Stockpile operator + (struct Stockpile const& obj) {
        struct Stockpile stockpile = {  this->Coal + obj.Coal,
                                        this->Oil + obj.Oil,
                                        this->Timber + obj.Timber,
                                        this->Rubber + obj.Rubber,
                                        this->Cotton + obj.Cotton,
                                        this->Iron + obj.Iron,
                                        this->Grain + obj.Grain,
                                        this->Fruit + obj.Fruit,
                                        this->Electric_gear + obj.Electric_gear,
                                        this->Machine_parts + obj.Machine_parts,
                                        this->Glass + obj.Glass,
                                        this->Lumber + obj.Lumber,
                                        this->Cement + obj.Cement,
                                        this->Ammunition + obj.Ammunition,
                                        this->Planes + obj.Planes,
                                        this->Explosives + obj.Explosives,
                                        this->Small_arms + obj.Small_arms,
                                        this->Artillery + obj.Artillery,
                                        this->Tanks + obj.Tanks,
                                        this->Canned_food + obj.Canned_food,
                                        this->Furniture + obj.Furniture,
                                        this->Clothes + obj.Clothes,
                                        this->Automobiles + obj.Automobiles,
                                        this->Merchant_ships + obj.Merchant_ships,
                                        this->Radios + obj.Radios,
                                        this->Telephones + obj.Telephones,
                                        this->Fuel + obj.Fuel,
                                        this->Paper + obj.Paper,
                                        this->Liquor + obj.Liquor,
                                        this->Airship + obj.Airship,
                                        this->Money + obj.Money };

        return stockpile;
    }

    void operator += (struct Stockpile const& obj) {
        this->Coal += obj.Coal;
        this->Oil += obj.Oil;
        this->Timber += obj.Timber;
        this->Rubber += obj.Rubber;
        this->Cotton += obj.Cotton;
        this->Iron += obj.Iron;
        this->Grain += obj.Grain;
        this->Fruit += obj.Fruit;
        this->Electric_gear += obj.Electric_gear;
        this->Machine_parts += obj.Machine_parts;
        this->Glass += obj.Glass;
        this->Lumber += obj.Lumber;
        this->Cement += obj.Cement;
        this->Ammunition += obj.Ammunition;
        this->Planes += obj.Planes;
        this->Explosives += obj.Explosives;
        this->Small_arms += obj.Small_arms;
        this->Artillery += obj.Artillery;
        this->Tanks += obj.Tanks;
        this->Canned_food += obj.Canned_food;
        this->Furniture += obj.Furniture;
        this->Clothes += obj.Clothes;
        this->Automobiles += obj.Automobiles;
        this->Merchant_ships += obj.Merchant_ships;
        this->Radios += obj.Radios;
        this->Telephones += obj.Telephones;
        this->Fuel += obj.Fuel;
        this->Paper += obj.Paper;
        this->Liquor += obj.Liquor;
        this->Airship += obj.Airship;
        this->Money += obj.Money;
    }

    struct Stockpile operator - (struct Stockpile const& obj) {
        struct Stockpile stockpile = { this->Coal - obj.Coal,
                                        this->Oil - obj.Oil,
                                        this->Timber - obj.Timber,
                                        this->Rubber - obj.Rubber,
                                        this->Cotton - obj.Cotton,
                                        this->Iron - obj.Iron,
                                        this->Grain - obj.Grain,
                                        this->Fruit - obj.Fruit,
                                        this->Electric_gear - obj.Electric_gear,
                                        this->Machine_parts - obj.Machine_parts,
                                        this->Glass - obj.Glass,
                                        this->Lumber - obj.Lumber,
                                        this->Cement - obj.Cement,
                                        this->Ammunition - obj.Ammunition,
                                        this->Planes - obj.Planes,
                                        this->Explosives - obj.Explosives,
                                        this->Small_arms - obj.Small_arms,
                                        this->Artillery - obj.Artillery,
                                        this->Tanks - obj.Tanks,
                                        this->Canned_food - obj.Canned_food,
                                        this->Furniture - obj.Furniture,
                                        this->Clothes - obj.Clothes,
                                        this->Automobiles - obj.Automobiles,
                                        this->Merchant_ships - obj.Merchant_ships,
                                        this->Radios - obj.Radios,
                                        this->Telephones - obj.Telephones,
                                        this->Fuel - obj.Fuel,
                                        this->Paper - obj.Paper,
                                        this->Liquor - obj.Liquor,
                                        this->Airship - obj.Airship,
                                        this->Money - obj.Money };

        return stockpile;
    }

    void operator -= (struct Stockpile const& obj) {
        this->Coal -= obj.Coal;
        this->Oil -= obj.Oil;
        this->Timber -= obj.Timber;
        this->Rubber -= obj.Rubber;
        this->Cotton -= obj.Cotton;
        this->Iron -= obj.Iron;
        this->Grain -= obj.Grain;
        this->Fruit -= obj.Fruit;
        this->Electric_gear -= obj.Electric_gear;
        this->Machine_parts -= obj.Machine_parts;
        this->Glass -= obj.Glass;
        this->Lumber -= obj.Lumber;
        this->Cement -= obj.Cement;
        this->Ammunition -= obj.Ammunition;
        this->Planes -= obj.Planes;
        this->Explosives -= obj.Explosives;
        this->Small_arms -= obj.Small_arms;
        this->Artillery -= obj.Artillery;
        this->Tanks -= obj.Tanks;
        this->Canned_food -= obj.Canned_food;
        this->Furniture -= obj.Furniture;
        this->Clothes -= obj.Clothes;
        this->Automobiles -= obj.Automobiles;
        this->Merchant_ships -= obj.Merchant_ships;
        this->Radios -= obj.Radios;
        this->Telephones -= obj.Telephones;
        this->Fuel -= obj.Fuel;
        this->Paper -= obj.Paper;
        this->Liquor -= obj.Liquor;
        this->Airship -= obj.Airship;
        this->Money -= obj.Money;
    }

} Stockpile;

#endif

