#ifndef FACTORY_H
#define FACTORY_H

#pragma once
#include "game/PerGood.h"
#include "game/Stockpile.h"

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
    SteelMill,
    AluminumSmelter,
    EngineFactory,
    BoilerFactory,
    CanvasMill,

    /*The two ways round a blockade. They come last because the Open Factory
    screen offers the kinds up to here and no further.*/
    SyntheticRubberRefinery,
    SyntheticNitratePlant,

    // Not a kind of factory: how many there are, so the table can be checked
    Count
};

// Everything that separates one kind of factory from another
struct FactoryKind {
    // The kind this row describes. It has to match the row's own position.
    FactoryType type;

    /*The good it exists to make. The factory is named and iconed after it, so
    the name is never written down twice.*/
    Good output;

    // What it costs in money to build one
    int cost = 0;

    /*What it costs in goods to build one. The four kinds that need nothing
    here - sawmill, kiln, glassworks, steelworks - are what a country with
    empty warehouses starts from, and every other kind is built out of what
    those four make. Without that footing no factory could ever be raised.*/
    Stockpile materials{};

    // How long a batch takes. Most factories turn one out every day.
    int daysToProduce = 1;

    // What one batch yields
    Stockpile produces{};

    // What a day of work at full throughput eats, as positive amounts
    Stockpile consumes{};

    /*What a day of work at full throughput would draw from the state's power
    supply. Nothing reads it yet - it is the hook the per-state energy system
    will balance against, written down now so that the recipes do not have to
    be opened up again when that lands.*/
    int powerDraw = 0;

    // What a day of work adds to that supply, for the power plants to come
    int powerOutput = 0;
};

/*The one place a factory's numbers are written down, in the order of the enum
above, which is also the order the Open Factory screen lists them in.*/
// clang-format off
inline constexpr std::array FactoryKinds{
    FactoryKind{.type = FactoryType::LumberMill, .output = Good::Lumber,
        .cost = 10000,
        .daysToProduce = 1, .produces = {.Lumber = 15}, .consumes = {.Coal = 2, .Timber = 10},
        .powerDraw = 6},

    FactoryKind{.type = FactoryType::GlassFactory, .output = Good::Glass,
        .cost = 8000,
        .daysToProduce = 1, .produces = {.Glass = 8}, .consumes = {.Coal = 4},
        .powerDraw = 6},

    FactoryKind{.type = FactoryType::CanningFactory, .output = Good::Canned_food,
        .cost = 8000, .materials = {.Steel = 30, .Machine_parts = 15, .Cement = 40},
        .daysToProduce = 1, .produces = {.Canned_food = 10}, .consumes = {.Coal = 2, .Grain = 3, .Steel = 2},
        .powerDraw = 5},

    FactoryKind{.type = FactoryType::ClothesFactory, .output = Good::Clothes,
        .cost = 11000, .materials = {.Steel = 30, .Machine_parts = 15, .Cement = 40},
        .daysToProduce = 1, .produces = {.Clothes = 10}, .consumes = {.Coal = 2, .Cotton = 8},
        .powerDraw = 5},

    FactoryKind{.type = FactoryType::LiquorDistillery, .output = Good::Liquor,
        .cost = 11000, .materials = {.Steel = 30, .Machine_parts = 15, .Cement = 40},
        .daysToProduce = 1, .produces = {.Liquor = 10}, .consumes = {.Coal = 2, .Fruit = 4, .Glass = 2},
        .powerDraw = 4},

    FactoryKind{.type = FactoryType::FurnitureFactory, .output = Good::Furniture,
        .cost = 12000, .materials = {.Steel = 30, .Machine_parts = 15, .Cement = 40},
        .daysToProduce = 1, .produces = {.Furniture = 7}, .consumes = {.Coal = 2, .Lumber = 10},
        .powerDraw = 5},

    FactoryKind{.type = FactoryType::AutomobileFactory, .output = Good::Automobiles,
        .cost = 16000, .materials = {.Steel = 150, .Machine_parts = 60, .Boilers = 6, .Cement = 120},
        .daysToProduce = 1, .produces = {.Automobiles = 3}, .consumes = {.Coal = 2, .Rubber = 1, .Steel = 4, .Engines = 1, .Glass = 1, .Lumber = 4},
        .powerDraw = 18},

    FactoryKind{.type = FactoryType::PaperMill, .output = Good::Paper,
        .cost = 8000, .materials = {.Steel = 30, .Machine_parts = 15, .Cement = 40},
        .daysToProduce = 1, .produces = {.Paper = 20}, .consumes = {.Coal = 2, .Lumber = 8},
        .powerDraw = 8},

    FactoryKind{.type = FactoryType::TelephoneFactory, .output = Good::Telephones,
        .cost = 16000, .materials = {.Steel = 60, .Machine_parts = 30, .Boilers = 2, .Cement = 60},
        .daysToProduce = 1, .produces = {.Telephones = 5}, .consumes = {.Coal = 2, .Copper = 1, .Electric_gear = 2},
        .powerDraw = 10},

    FactoryKind{.type = FactoryType::RadioFactory, .output = Good::Radios,
        .cost = 15000, .materials = {.Steel = 60, .Machine_parts = 30, .Boilers = 2, .Cement = 60},
        .daysToProduce = 1, .produces = {.Radios = 5}, .consumes = {.Coal = 2, .Copper = 1, .Electric_gear = 2},
        .powerDraw = 10},

    FactoryKind{.type = FactoryType::MachinePartFactory, .output = Good::Machine_parts,
        .cost = 20000, .materials = {.Steel = 60, .Cement = 50},
        .daysToProduce = 1, .produces = {.Machine_parts = 6}, .consumes = {.Coal = 2, .Steel = 8},
        .powerDraw = 15},

    FactoryKind{.type = FactoryType::ElectricGearFactory, .output = Good::Electric_gear,
        .cost = 19000, .materials = {.Steel = 60, .Machine_parts = 30, .Boilers = 2, .Cement = 60},
        .daysToProduce = 1, .produces = {.Electric_gear = 8}, .consumes = {.Coal = 2, .Copper = 4, .Steel = 2},
        .powerDraw = 20},

    FactoryKind{.type = FactoryType::FuelRefinery, .output = Good::Fuel,
        .cost = 22000, .materials = {.Steel = 60, .Machine_parts = 30, .Boilers = 2, .Cement = 60},
        .daysToProduce = 1, .produces = {.Fuel = 8}, .consumes = {.Coal = 2, .Oil = 4},
        .powerDraw = 10},

    FactoryKind{.type = FactoryType::CementFactory, .output = Good::Cement,
        .cost = 12000,
        .daysToProduce = 1, .produces = {.Cement = 10}, .consumes = {.Coal = 4},
        .powerDraw = 6},

    FactoryKind{.type = FactoryType::Shipyard, .output = Good::Merchant_ships,
        .cost = 19000, .materials = {.Steel = 150, .Machine_parts = 60, .Boilers = 6, .Cement = 120},
        .daysToProduce = 120, .produces = {.Merchant_ships = 1}, .consumes = {.Coal = 2, .Rubber = 1, .Steel = 4, .Boilers = 1, .Glass = 1, .Lumber = 4, .Radios = 1},
        .powerDraw = 20},

    FactoryKind{.type = FactoryType::SmallArmsFactory, .output = Good::Small_arms,
        .cost = 16000, .materials = {.Steel = 60, .Machine_parts = 30, .Boilers = 2, .Cement = 60},
        .daysToProduce = 1, .produces = {.Small_arms = 15}, .consumes = {.Coal = 2, .Steel = 8, .Lumber = 8},
        .powerDraw = 12},

    FactoryKind{.type = FactoryType::AmmunitionFactory, .output = Good::Ammunition,
        .cost = 14000, .materials = {.Steel = 60, .Machine_parts = 30, .Boilers = 2, .Cement = 60},
        .daysToProduce = 1, .produces = {.Ammunition = 5}, .consumes = {.Coal = 2, .Steel = 3, .Explosives = 2},
        .powerDraw = 10},

    FactoryKind{.type = FactoryType::ArtilleryFactory, .output = Good::Artillery,
        .cost = 19000, .materials = {.Steel = 150, .Machine_parts = 60, .Boilers = 6, .Cement = 120},
        .daysToProduce = 1, .produces = {.Artillery = 3}, .consumes = {.Coal = 2, .Steel = 6, .Machine_parts = 1},
        .powerDraw = 16},

    /*Nitrates rather than coal: an explosive is fixed nitrogen, and that is
    what a blockade was for.*/
    FactoryKind{.type = FactoryType::ExplosivesFactory, .output = Good::Explosives,
        .cost = 13000, .materials = {.Steel = 60, .Machine_parts = 30, .Boilers = 2, .Cement = 60},
        .daysToProduce = 1, .produces = {.Explosives = 6}, .consumes = {.Coal = 2, .Nitrates = 4},
        .powerDraw = 14},

    FactoryKind{.type = FactoryType::TankFactory, .output = Good::Tanks,
        .cost = 30000, .materials = {.Steel = 150, .Machine_parts = 60, .Boilers = 6, .Cement = 120},
        .daysToProduce = 1, .produces = {.Tanks = 1}, .consumes = {.Coal = 2, .Steel = 4, .Engines = 1, .Small_arms = 2, .Artillery = 1},
        .powerDraw = 20},

    /*A rigid airship is a duralumin frame under a doped fabric skin, which is
    what the aluminium and the canvas are doing here.*/
    FactoryKind{.type = FactoryType::AirshipFactory, .output = Good::Airship,
        .cost = 40000, .materials = {.Steel = 150, .Machine_parts = 60, .Boilers = 6, .Cement = 120},
        .daysToProduce = 200, .produces = {.Airship = 1}, .consumes = {.Coal = 2, .Aluminum = 4, .Electric_gear = 2, .Engines = 2, .Canvas = 12},
        .powerDraw = 18},

    /*Wood, doped fabric and an engine. There is no aluminium in a 1914
    airframe - that is what the airships were for.*/
    FactoryKind{.type = FactoryType::PlaneFactory, .output = Good::Planes,
        .cost = 24000, .materials = {.Steel = 150, .Machine_parts = 60, .Boilers = 6, .Cement = 120},
        .daysToProduce = 1, .produces = {.Planes = 2}, .consumes = {.Coal = 2, .Rubber = 2, .Engines = 1, .Lumber = 4, .Canvas = 3},
        .powerDraw = 12},

    FactoryKind{.type = FactoryType::SteelMill, .output = Good::Steel,
        .cost = 18000,
        .daysToProduce = 1, .produces = {.Steel = 10}, .consumes = {.Coal = 6, .Iron = 12},
        .powerDraw = 30},

    /*A smelter is really an appetite for electricity with a factory attached.
    Until power is metered on its own the coal stands in for it, and powerDraw
    is what it will cost once it is.*/
    FactoryKind{.type = FactoryType::AluminumSmelter, .output = Good::Aluminum,
        .cost = 26000, .materials = {.Steel = 150, .Machine_parts = 60, .Boilers = 6, .Cement = 120},
        .daysToProduce = 1, .produces = {.Aluminum = 4}, .consumes = {.Coal = 14, .Bauxite = 8},
        .powerDraw = 120},

    FactoryKind{.type = FactoryType::EngineFactory, .output = Good::Engines,
        .cost = 24000, .materials = {.Steel = 60, .Machine_parts = 30, .Boilers = 2, .Cement = 60},
        .daysToProduce = 1, .produces = {.Engines = 3}, .consumes = {.Coal = 2, .Copper = 2, .Steel = 6, .Machine_parts = 4},
        .powerDraw = 15},

    FactoryKind{.type = FactoryType::BoilerFactory, .output = Good::Boilers,
        .cost = 21000, .materials = {.Steel = 60, .Cement = 50},
        .daysToProduce = 1, .produces = {.Boilers = 2}, .consumes = {.Coal = 3, .Steel = 12, .Machine_parts = 2},
        .powerDraw = 12},

    FactoryKind{.type = FactoryType::CanvasMill, .output = Good::Canvas,
        .cost = 10000, .materials = {.Steel = 30, .Machine_parts = 15, .Cement = 40},
        .daysToProduce = 1, .produces = {.Canvas = 12}, .consumes = {.Coal = 2, .Cotton = 10},
        .powerDraw = 6},

    FactoryKind{.type = FactoryType::SyntheticRubberRefinery, .output = Good::Rubber,
        .cost = 14000, .materials = {.Steel = 30, .Machine_parts = 15, .Cement = 40},
        .daysToProduce = 1, .produces = {.Rubber = 6}, .consumes = {.Coal = 5},
        .powerDraw = 12},

    /*Nitrates out of coal and air rather than out of a Chilean mine. Fixing
    nitrogen is hungry work, which is what the draw is for.*/
    FactoryKind{.type = FactoryType::SyntheticNitratePlant, .output = Good::Nitrates,
        .cost = 20000, .materials = {.Steel = 60, .Machine_parts = 30, .Boilers = 2, .Cement = 60},
        .daysToProduce = 1, .produces = {.Nitrates = 5}, .consumes = {.Coal = 8},
        .powerDraw = 90},
};
// clang-format on

/*The table is subscripted by FactoryType, so every row has to sit at its own
enumerator and every enumerator has to have a row.*/
static_assert(FactoryKinds.size() == std::size_t(FactoryType::Count));
static_assert([] {
    for(std::size_t i = 0; i < FactoryKinds.size(); ++i) {
        if(FactoryKinds[i].type != FactoryType(i)) return false;
    }
    return true;
}());

/*A factory is named after the one good it turns out, so every row has to
produce the good it claims to.*/
static_assert([] {
    for(const auto& kind : FactoryKinds) {
        if(kind.produces[kind.output] <= 0) return false;
    }
    return true;
}());

// What a kind of factory costs, makes and eats
inline constexpr const FactoryKind& KindOf(FactoryType type) {
    return FactoryKinds[std::size_t(type)];
}

// What a kind of factory is called, which is the name of its icon too
inline constexpr std::string_view NameOf(FactoryType type) {
    return InfoOf(KindOf(type).output).name;
}

/*A day of work is measured in thousandths, so that a factory short of an input
can run at a fraction of a day rather than having to stall or run whole.*/
inline constexpr int FullThroughput = 1000;

class Factory {
public:
    explicit Factory(FactoryType type);

    /*A day of work at some fraction of full throughput, in thousandths. The
    inputs are taken out of the stockpile, the batch is handed over on the day
    it is finished.*/
    void Work(int throughput, Stockpile& stock);

    // What a full day of work would eat, which is what the day's demand is built from
    Stockpile Consumption() const;

    /*The most of a day's work the stockpile can pay for, in thousandths, given
    how far each good's supply went round. A factory runs at the rate of its
    scarcest input, so this is the smallest share among the goods it needs.

    This is where the per-state power supply will come in: a factory will run at
    the lesser of what its inputs and what its state's grid allow.*/
    int Throughput(const PerGood<int>& share) const;

    // What this factory is, and every number that follows from it
    const FactoryKind& Kind;

    // How many batches it runs at once
    int size;

private:
    /*How far the batch under way has got, in thousandths of a day of work, so
    that a factory held back by a shortage takes proportionally longer rather
    than stalling outright.*/
    int progress;
};
#endif
