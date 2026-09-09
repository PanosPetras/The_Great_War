#include "game/Factory.h"

#include <algorithm>

Factory::Factory(FactoryType type) : Kind{KindOf(type)}, size{1}, progress{0} {}

Stockpile Factory::Consumption() const {
    return Kind.consumes * size;
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

void Factory::Work(int throughput, Stockpile& stock) {
    if(throughput <= 0) return;

    /*Take the share of a full day's inputs that the day's work used. The share
    was worked out so that everything claiming a good together claims no more
    than there is, so this can never take the stockpile below zero.*/
    const Stockpile wanted = Consumption();
    for(auto good : AllGoods) {
        stock[good] -= wanted[good] * throughput / FullThroughput;
    }

    /*A batch is finished once daysToProduce full days of work have gone into
    it. What is left over carries into the next one, so a shortage makes a
    factory slow rather than throwing the part-built batch away.*/
    progress += throughput;

    if(const int batch = FullThroughput * Kind.daysToProduce; progress >= batch) {
        progress -= batch;
        stock += Kind.produces * size;
    }
}
