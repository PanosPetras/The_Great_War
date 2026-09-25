#ifndef POPNEEDS_H
#define POPNEEDS_H

#include "game/PerGood.h"

#include <array>

/*What a country's people take out of its warehouses each day, and how much
each good counts towards how well they live.

Amounts are per ten million people, a scale the rest of the economy can meet:
a canning factory turns out ten tins a day, so a need written per head would be
millions of tins and could never be met. Grain and fruit are pitched against
what the world digs in 1910, which just about feeds it - so a country that
grows its own food starts out fed, and one that does not, Britain first among
them, starts out short.

Weights add up to a hundred, so each one reads as the percentage of the
standard of living that good carries. Food is two fifths of it: a country with
full granaries and no industry at all lives at 40%.*/
struct PopNeed {
    Good good;

    // What ten million people get through in a day
    int perTenMillion;

    // How much going without it hurts, out of a hundred
    int weight;
};

inline constexpr long long TenMillion = 10'000'000;

// clang-format off
inline constexpr std::array PopNeeds{
    // Bread, and what goes with it
    PopNeed{Good::Grain, 300, 30},
    PopNeed{Good::Fruit, 80, 10},

    // What a household buys week to week
    PopNeed{Good::Canned_food, 30, 8},
    PopNeed{Good::Clothes, 30, 10},
    PopNeed{Good::Paper, 20, 4},
    PopNeed{Good::Liquor, 10, 4},

    // Building and furnishing a home
    PopNeed{Good::Lumber, 20, 5},
    PopNeed{Good::Cement, 10, 4},
    PopNeed{Good::Glass, 10, 3},
    PopNeed{Good::Furniture, 10, 6},

    // The new century
    PopNeed{Good::Automobiles, 2, 5},
    PopNeed{Good::Fuel, 5, 4},
    PopNeed{Good::Telephones, 3, 4},
    PopNeed{Good::Radios, 2, 3},
};
// clang-format on

static_assert([] {
    int total = 0;
    for(const auto& need : PopNeeds) total += need.weight;
    return total == 100;
}());

/*How well a country lives, in thousandths: every need's weight times the
share of it that was met. A country with no people has nothing to want and
counts as living well. Before the first day's needs are met, a country is
taken to be fed and nothing more.*/
inline constexpr int SubsistenceSatisfaction = 400;

/*How fast the population grows at a given standard of living, in thousandths
of the rate policy alone would give. Fed and nothing more is the old rate.
Below that, growth falls away to nothing; above it, better living buys up to
half as much growth again.

Hunger stalls a population rather than shrinking it. Norway and Sweden dig
nowhere near enough grain for themselves, and until they can buy it abroad a
decline would be a sentence rather than a pressure.*/
inline constexpr int GrowthPermille(int satisfaction) {
    if(satisfaction <= SubsistenceSatisfaction) {
        return satisfaction * 1000 / SubsistenceSatisfaction;
    }
    return 1000 + (satisfaction - SubsistenceSatisfaction) * 500 / (1000 - SubsistenceSatisfaction);
}

static_assert(GrowthPermille(0) == 0);
static_assert(GrowthPermille(SubsistenceSatisfaction) == 1000);
static_assert(GrowthPermille(1000) == 1500);

#endif
