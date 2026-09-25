#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include "game/PerGood.h"

/*How far a country's know-how multiplies the work of its factories, mines,
farms and forests. Every field is in thousandths, the same unit a day of
factory work is measured in, so 1000 is the 1910 baseline and 1100 is a tenth
better. Research is what will move these; until it exists they sit at 1000.*/
struct Technology {
    // What a factory eats for the same day of work. Lower is better.
    int FactoryInput = 1000;

    // How much work a factory gets done in a day, inputs and all
    int FactoryThroughput = 1000;

    // What a finished batch yields
    int FactoryOutput = 1000;

    // What comes out of mines and wells: coal, oil, the ores and the nitrates
    int MineralOutput = 1000;

    // What comes out of fields and plantations: grain, fruit, cotton and rubber
    int FarmOutput = 1000;

    // What comes out of forests
    int WoodOutput = 1000;

    // Which of the three extraction multipliers applies to a raw good
    constexpr int ExtractionOf(Good good) const {
        switch(good) {
        case Good::Timber:
            return WoodOutput;
        case Good::Rubber:
        case Good::Cotton:
        case Good::Grain:
        case Good::Fruit:
            return FarmOutput;
        default:
            return MineralOutput;
        }
    }
};

/*An amount scaled by a multiplier in thousandths, to the nearest whole unit.
The rounding matters: truncating would make a tenth better at a two-a-day
input worth nothing at all.*/
inline constexpr int ScaleByPermille(int amount, int permille) {
    return int((static_cast<long long>(amount) * permille + 500) / 1000);
}

// Every good in a table scaled the same way
inline constexpr PerGood<int> ScaleByPermille(PerGood<int> amounts, int permille) {
    for(auto good : AllGoods) {
        amounts[good] = ScaleByPermille(amounts[good], permille);
    }
    return amounts;
}

#endif
