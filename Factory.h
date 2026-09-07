#ifndef FACTORY_H
#define FACTORY_H

#pragma once
#include "Market.h"
#include "Stockpile.h"

#include <array>
#include <cstddef>
#include <string_view>

/*Every kind of factory the game knows. The table below holds one row per
enumerator, in this order, so a kind is both a name to pass around and the
subscript of its own numbers.*/
enum class FactoryType {
    LumberMill,
    GlassFactory,
    CanningFactory,
    ClothesFactory,
    LiquorDistillery,
    FurnitureFactory,
    AutomobileFactory,
    PaperMill,
    TelephoneFactory,
    RadioFactory,
    MachinePartFactory,
    ElectricGearFactory,
    FuelRefinery,
    CementFactory,
    Shipyard,
    SmallArmsFactory,
    AmmunitionFactory,
    ArtilleryFactory,
    ExplosivesFactory,
    TankFactory,
    AirshipFactory,
    PlaneFactory,
    SyntheticRubberRefinery,

    // Not a kind of factory: how many there are, so the table can be checked
    Count
};

// Everything that separates one kind of factory from another
struct FactoryKind {
    // The kind this row describes. It has to match the row's own position.
    FactoryType type;

    /*What the factory turns out, in lower case. It doubles as the name of the
    icon that stands for the factory under Icons/Goods.*/
    std::string_view name;

    // What it costs to build one
    int cost;

    // How long a batch takes. Most factories turn one out every day.
    int daysToProduce;

    // What one batch yields
    Stockpile produces;

    // What a day of work eats, as positive amounts
    Stockpile consumes;
};

/*The one place a factory's numbers are written down, in the order of the enum
above, which is also the order the Open Factory screen lists them in.*/
inline constexpr std::array FactoryKinds{
    FactoryKind{FactoryType::LumberMill, "lumber", 10000, 1, {.Lumber = 15}, {.Coal = 2, .Timber = 10}},
    FactoryKind{FactoryType::GlassFactory, "glass", 8000, 1, {.Glass = 8}, {.Coal = 4}},
    FactoryKind{FactoryType::CanningFactory, "canned food", 8000, 1, {.Canned_food = 10}, {.Coal = 2, .Iron = 2, .Grain = 3}},
    FactoryKind{FactoryType::ClothesFactory, "clothes", 11000, 1, {.Clothes = 10}, {.Coal = 2, .Cotton = 8}},
    FactoryKind{FactoryType::LiquorDistillery, "liquor", 11000, 1, {.Liquor = 10}, {.Coal = 2, .Fruit = 4, .Glass = 2}},
    FactoryKind{FactoryType::FurnitureFactory, "furniture", 12000, 1, {.Furniture = 7}, {.Coal = 2, .Lumber = 10}},
    FactoryKind{FactoryType::AutomobileFactory, "automobile", 16000, 1, {.Automobiles = 3}, {.Coal = 2, .Rubber = 1, .Iron = 4, .Glass = 1, .Lumber = 4}},
    FactoryKind{FactoryType::PaperMill, "paper", 8000, 1, {.Paper = 20}, {.Coal = 2, .Lumber = 8}},
    FactoryKind{FactoryType::TelephoneFactory, "telephone", 16000, 1, {.Telephones = 5}, {.Coal = 2, .Electric_gear = 2}},
    FactoryKind{FactoryType::RadioFactory, "radio", 15000, 1, {.Radios = 5}, {.Coal = 2, .Electric_gear = 2}},
    FactoryKind{FactoryType::MachinePartFactory, "machine parts", 20000, 1, {.Machine_parts = 6}, {.Coal = 2, .Iron = 12}},
    FactoryKind{FactoryType::ElectricGearFactory, "electric gear", 19000, 1, {.Electric_gear = 8}, {.Coal = 2, .Iron = 4}},
    FactoryKind{FactoryType::FuelRefinery, "fuel", 22000, 1, {.Fuel = 8}, {.Coal = 2, .Oil = 4}},
    FactoryKind{FactoryType::CementFactory, "cement", 12000, 1, {.Cement = 10}, {.Coal = 4}},
    FactoryKind{FactoryType::Shipyard, "merchant ship", 19000, 120, {.Merchant_ships = 1}, {.Coal = 2, .Rubber = 1, .Iron = 4, .Glass = 1, .Lumber = 4}},
    FactoryKind{FactoryType::SmallArmsFactory, "small arms", 16000, 1, {.Small_arms = 15}, {.Coal = 2, .Iron = 8, .Lumber = 8}},
    FactoryKind{FactoryType::AmmunitionFactory, "ammunition", 14000, 1, {.Ammunition = 5}, {.Coal = 2, .Iron = 3}},
    FactoryKind{FactoryType::ArtilleryFactory, "artillery", 19000, 1, {.Artillery = 3}, {.Coal = 2, .Iron = 3}},
    FactoryKind{FactoryType::ExplosivesFactory, "explosives", 13000, 1, {.Explosives = 6}, {.Coal = 2}},
    FactoryKind{FactoryType::TankFactory, "tank", 30000, 1, {.Tanks = 1}, {.Coal = 2, .Iron = 4, .Small_arms = 2, .Artillery = 1}},
    FactoryKind{FactoryType::AirshipFactory, "airship", 40000, 200, {.Airship = 1}, {.Coal = 2, .Cotton = 12, .Iron = 10, .Electric_gear = 2}},
    FactoryKind{FactoryType::PlaneFactory, "plane", 24000, 1, {.Planes = 2}, {.Coal = 2, .Rubber = 2, .Iron = 1, .Lumber = 4}},
    FactoryKind{FactoryType::SyntheticRubberRefinery, "rubber", 14000, 1, {.Rubber = 6}, {.Coal = 5}},
};

/*The table is subscripted by FactoryType, so every row has to sit at its own
enumerator and every enumerator has to have a row.*/
static_assert(FactoryKinds.size() == std::size_t(FactoryType::Count));
static_assert([] {
    for(std::size_t i = 0; i < FactoryKinds.size(); ++i) {
        if(FactoryKinds[i].type != FactoryType(i)) return false;
    }
    return true;
}());

// What a kind of factory costs, makes and eats
inline constexpr const FactoryKind& KindOf(FactoryType type) {
    return FactoryKinds[std::size_t(type)];
}

struct Factory {
public:
    Factory(FactoryType type, Stockpile* Target, Market* market);
    ~Factory();

    /*A day of work. The inputs are taken every day, the batch is handed over on
    the day it is finished.*/
    void Tick();

    void ChangeOwner(Stockpile* NewStockpile);

    // What this factory is, and every number that follows from it
    const FactoryKind& Kind;

    // How many batches it runs at once
    int size;

private:
    Stockpile materialsNeeded;
    Stockpile materialsProduced;
    Stockpile* TargetStockpile;

    Market* TargetMarket;

    // Days spent on the batch under way
    int days;

    void confirmMaterials();
};
#endif
