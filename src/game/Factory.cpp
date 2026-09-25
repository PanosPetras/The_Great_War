#include "game/Factory.h"

#include <algorithm>

Factory::Factory(FactoryType type) : Kind{KindOf(type)}, size{1}, progress{0} {}

long long Factory::RunningCost(int throughput) const {
    const long long cost = static_cast<long long>(Kind.cost) * size;
    return cost / UpkeepDivisor + cost * std::max(0, throughput) / (WagesDivisor * FullThroughput);
}

Stockpile Factory::Consumption(const Technology& tech) const {
    return ScaleByPermille(Kind.consumes * size, tech.FactoryInput * tech.FactoryThroughput / 1000);
}

int Factory::Throughput(const PerGood<int>& share) const {
    int throughput = FullThroughput;

    for(auto good : AllGoods) {
        if(Kind.consumes[good] > 0) {
            throughput = std::min(throughput, share[good]);
        }
    }

    return throughput;
}

void Factory::Work(int throughput, Stockpile& stock, const Technology& tech) {
    if(throughput <= 0) return;

    /*Take the share of a full day's inputs that the day's work used. The share
    was worked out so that everything claiming a good together claims no more
    than there is, so this can never take the stockpile below zero.*/
    const Stockpile wanted = Consumption(tech);
    for(auto good : AllGoods) {
        stock[good] -= wanted[good] * throughput / FullThroughput;
    }

    /*A batch is finished once daysToProduce full days of work have gone into
    it. What is left over carries into the next one, so a shortage makes a
    factory slow rather than throwing the part-built batch away.*/
    progress += ScaleByPermille(throughput, tech.FactoryThroughput);

    if(const int batch = FullThroughput * Kind.daysToProduce; progress >= batch) {
        progress -= batch;
        stock += ScaleByPermille(Kind.produces * size, tech.FactoryOutput);
    }
}
