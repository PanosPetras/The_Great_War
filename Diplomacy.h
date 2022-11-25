#ifndef DIPLOMACY_H
#define DIPLOMACY_H

#pragma once
#include <vector>
#include <string>
#include <unordered_map>

class Country;

struct War {
    std::vector<Country*> Side1;
    std::vector<Country*> Side2;
    int Score;
};

struct TradeDeal {
    float offset;

    void End();
};

struct Embargo {
    Country* Instigator;
};

struct Relations {
public:
    Country* sides[2];

    Relations();
    Relations(Country* country1, Country* country2, int relations, bool allied = false);

    void ImproveRelations();
    void WorsenRelations();
    int GetRelationsValue();

    void CreateAlliance();
    void BreakAlliance();
    bool GetIfAllied();

    std::vector<std::string> toString();

private:
    const int limit = 300;
    std::vector<Embargo> Embargoes;

    int relationsValue;
    bool allied;
};

struct Diplomacy {
public:
    Diplomacy();
    ~Diplomacy();

    std::unordered_map<std::string, Relations*>* relations;
    std::vector<War> wars;

    Relations* findRelation(std::vector<std::string> ids);
};

#endif
