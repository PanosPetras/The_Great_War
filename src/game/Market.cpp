#include "game/Market.h"
#include "game/Country.h"

#include <algorithm>

Market::Market() {
    for(auto good : AllGoods) {
        Price[good] = InfoOf(good).basePrice;
    }
}

void Market::Clear(const std::vector<std::unique_ptr<Country>>& countries) {
    /*Every order is placed before any is filled, all at the same prices and
    against the same view of what was on offer yesterday.*/
    std::vector<TradeOrders> orders;
    orders.reserve(countries.size());
    for(const auto& country : countries) {
        orders.push_back(country->PlaceOrders(*this));
    }

    for(auto good : AllGoods) {
        long long supply = 0;
        long long demand = 0;
        for(const auto& order : orders) {
            supply += order.offers[good];
            demand += order.bids[good];
        }

        /*Sellers are rationed first, each offer to the same fraction of
        itself, rounded down. Buyers then share out what the sellers actually
        handed over, the same way. Rounding both sides down against the same
        total would let the buyers receive a unit or two that no seller gave
        up; this way a good can be lost to rounding, but never made out of
        nothing, and nobody sells more than they offered or is handed more than
        they bid for.*/
        const long long traded = std::min(supply, demand);

        std::vector<int> sold(countries.size());
        long long delivered = 0;
        if(traded > 0) {
            for(std::size_t i = 0; i < countries.size(); ++i) {
                sold[i] = int(orders[i].offers[good] * traded / supply);
                delivered += sold[i];
            }
            for(std::size_t i = 0; i < countries.size(); ++i) {
                const int bought = int(orders[i].bids[good] * delivered / demand);
                if(sold[i] > 0 || bought > 0) {
                    countries[i]->Settle(good, sold[i], bought, Price[good]);
                }
            }
        }

        Supply[good] = int(supply);
        Demand[good] = int(demand);
        Traded[good] = int(delivered);

        // The side that went unsatisfied pulls the price its way
        if(supply + demand > 0) {
            const double base = InfoOf(good).basePrice;
            Price[good] *= 1.0 + PriceStep * double(demand - supply) / double(demand + supply);
            Price[good] = std::clamp(Price[good], base / PriceRange, base * PriceRange);
        }
    }
}
