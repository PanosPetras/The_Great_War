#ifndef DIPLOMACY_H
#define DIPLOMACY_H

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include "Country.h"

#define RELATIONS_LIMIT 300

struct CountryPair {
public:
    CountryPair(Country* C1, Country* C2);

    bool operator == (const CountryPair& c) const;

    Country* GetC1() const;
    Country* GetC2() const;

private:
    Country *c1, *c2;
};

template<>
struct std::hash<CountryPair> {
    std::size_t operator()(const CountryPair& k) const {
        std::hash<std::string> hasher;

        std::string str1 = k.GetC1()->GetTag(), str2 = k.GetC2()->GetTag();

        return hasher(str1 > str2 ? str1 + str2 : str2 + str1);
    }
};

struct War {
    std::vector<Country*> Side1;
    std::vector<Country*> Side2;
    int Score;
};

struct Embargo {
    Country* Instigator;
};

struct Relations {
public:
    Relations(int relations, bool allied = false);

    void ImproveRelations();
    void WorsenRelations();
    int GetRelationsValue() const;

    void CreateAlliance();
    void BreakAlliance();
    bool GetIfAllied() const;

private:
    std::vector<Embargo> Embargoes;

    int relationsValue;
    bool allied;
};

struct Diplomacy {
public:
    Diplomacy();
    ~Diplomacy();

    std::unordered_map<CountryPair, Relations>* relations;
    std::vector<War> wars;
};

#endif