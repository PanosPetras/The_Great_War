#ifndef DIPLOMACY_H
#define DIPLOMACY_H

#pragma once
#include <vector>
#include <string>

class Country;

struct War {
    std::vector<Country*> Side1;
    std::vector<Country*> Side2;
    int Score;
};

struct TradeDeal {
    float offset;
    Country* sides[2];

    void End();
};

struct Embargo {
    Country* sides[2];

    void End();
};

struct Relations {
    Country* sides[2];
    int value;

    void improveRelations();
    void worsenRelations();
};

struct Diplomacy {
    std::vector<Relations> relations;
    std::vector<TradeDeal> tradeDeals;
    std::vector<Embargo> embargoes;
    std::vector<std::string> alliances;
    std::vector<War> wars;
};

#endif
