#ifndef market_h
#define market_h

#include "game/PerGood.h"
#include "game/Stockpile.h"

#include <memory>
#include <vector>

class Country;

/*The one world market every country trades on. Once a day, after every country
has had its day at home, each one puts up what it will sell and what it wants
to buy, every good is cleared at today's price, and the price then moves
towards whichever side was left over.

Clearing is proportional, like the factories' share of a warehouse: when
sellers bring more than buyers want, every seller sells the same fraction of
their offer, and the other way round for buyers. No country is served before
another, so the order the countries are held in cannot matter.*/
class Market {
public:
    Market();

    // A day of trade among these countries
    void Clear(const std::vector<std::unique_ptr<Country>>& countries);

    // What a unit of each good costs today
    PerGood<double> Price;

    // Yesterday's trade: what was offered, what was asked for, and what changed hands
    Stockpile Supply;
    Stockpile Demand;
    Stockpile Traded;

private:
    /*How far a price moves in a day when one side of its market is all there
    is, as a fraction of the price. With twice as much on offer as is asked for,
    it moves a third as far.*/
    static constexpr double PriceStep = 0.02;

    // How far a price may wander from its base, either way
    static constexpr double PriceRange = 4.0;
};

#endif
