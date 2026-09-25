#ifndef COUNTRY_H
#define COUNTRY_H

#include "game/State.h"
#include "game/Stockpile.h"
#include "game/Technology.h"
#include <string>
#include <vector>

class Market;
class Request;

/*Where yesterday's money came from and went, for the screens that show it.
Every figure is positive; the country's balance moved by what came in, taxes
and exports, minus what went out.*/
struct Budget {
    long long taxes = 0;
    long long healthcare = 0;
    long long factories = 0;

    // What the world market paid for what the country sold, and was paid for what it bought
    long long exports = 0;
    long long imports = 0;

    long long Net() const { return taxes + exports - healthcare - factories - imports; }
};

// What a country puts to the world market in a day
struct TradeOrders {
    // What it will sell, in units
    Stockpile offers{};

    // What it will buy, in units
    Stockpile bids{};
};

class Policy {
public:
    int TaxRate;
    int Healthcare;
};

class Country {
public:
    // Functions

    // Constructor
    Country(std::string tag, std::string name, const Stockpile& sp, long long money, bool isPlayerControlled, Color rgb);
    Country(std::string tag, std::string name, const Stockpile& sp, long long money, Color rgb = Color{});

    // Handle the states of the country
    void AddState(State* state);
    void RemoveState(State* state);

    /*Hands one of this country's states to another: its output, its factories
    and its people stop counting here and start counting there.*/
    void CedeState(State* state, Country& to);

    // This is the representing the pass of a single day
    void Tick();

    void AddRequest(Request request);

    // Accessor Functions
    std::string GetName() const;
    std::string GetTag() const;

    /*Everyone living in the states the country holds today. Worked out on
    asking, because every state's population grows every day.*/
    int GetPopulation() const;

    /*How well the people lived yesterday, in thousandths: the weighted share
    of their needs the warehouses could meet. See PopNeeds.h.*/
    int GetSatisfaction() const;

    // Yesterday's income and spending
    const Budget& GetBudget() const;

    /*What the country got through yesterday: what its factories would eat at
    full throughput plus what its people need. Trade keeps a reserve of it.*/
    const Stockpile& GetDailyNeed() const;

    /*What the country will sell and buy today on this market. It keeps a
    month of its daily need, and enough to raise a couple of factories, and
    offers a tenth of anything above that; it bids for a tenth of whatever it
    is short of a month's need, and spends no more than a tenth of its
    treasury in a day doing it, or today's taxes if those are more. Arms are
    never sold off automatically.

    It never bids for more than the whole world offered yesterday. Otherwise a
    poor country's purse is shared out over goods nobody makes, and the grain it
    could have bought is scaled down to nothing.*/
    TradeOrders PlaceOrders(const Market& market) const;

    // Hands over what the market cleared of one good, at the price it cleared at
    void Settle(Good good, int sold, int bought, double price);
    bool GetIfIsPlayer() const;

private:
    Color color;

    // Basic Properties
    bool isPlayer;
    std::string name;
    std::string tag;

    /*The states the country controls, whose output lands in its warehouses and
    whose factories it runs. A list rather than a map by name: seven pairs of
    states share a name, and a map kept only one of each.*/
    std::vector<State*> ownedStates;

    // The diplomatic requests that a country receives
    std::vector<Request> requests;

    void HandleDiplomaticRequests();

    /*A day of work in every factory the country runs. The whole country draws
    on one set of warehouses, so the factories cannot be run one at a time: the
    day's demand is added up first, then each good's stock is divided among
    everything that wants it, and only then does any factory take anything. A
    factory that cannot have all of its inputs runs at the rate of its
    scarcest one instead of stopping, and nothing can be spent twice.

    Power is the one input that is not shared country-wide: each state's
    factories run on what its own power stations make, and a state that draws
    more than it makes runs every powered factory in it proportionally slower.*/
    void RunFactories();

    /*Collects the day's taxes and pays for healthcare. What people can be
    taxed for depends on how well they live, so a country that makes nothing
    its people want collects little from them.*/
    void CollectTaxes();

    /*The people take what they need out of whatever the factories left, and
    how much of it there was becomes the day's satisfaction. They come after
    the factories: industry has first call on the warehouses, so a country
    that builds too hungrily will feel it at home.*/
    void FeedPopulation();

    int satisfaction;

    Budget budget;

    Stockpile dailyNeed;

public:
    Policy policy;

    Technology technology;

    // A country's currently stockpiled resources
    Stockpile Stock;

    // What it has in the bank
    long long Money;
};

#endif
