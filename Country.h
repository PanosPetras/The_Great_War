#ifndef COUNTRY_H
#define COUNTRY_H

#include "State.h"
#include "Stockpile.h"
#include <string>
#include <unordered_map>
#include <vector>

class Request;

class Technology {
public:
    float FactoryInput;
    float FactoryThroughput;
    float FactoryOutput;
    float MineralOutput;
    float FarmOutput;
    float WoodOutput;
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

    // This is the representing the pass of a single day
    void Tick();

    void AddRequest(Request request);

    // Accessor Functions
    std::string GetName() const;
    std::string GetTag() const;
    int GetPopulation() const;
    bool GetIfIsPlayer() const;

private:
    Color color;

    // Basic Properties
    bool isPlayer;
    std::string name;
    std::string tag;
    int population;

    int stateCount;
    std::unordered_map<std::string, State*> ownedStates;

    Technology technology;

    // The diplomatic requests that a country receives
    std::vector<Request> requests;

    void HandleDiplomaticRequests();

    /*A day of work in every factory the country runs. The whole country draws
    on one set of warehouses, so the factories cannot be run one at a time: the
    day's demand is added up first, then each good's stock is divided among
    everything that wants it, and only then does any factory take anything. A
    factory that cannot have all of its inputs runs at the rate of its
    scarcest one instead of stopping, and nothing can be spent twice.*/
    void RunFactories();

public:
    Policy policy;

    // A country's currently stockpiled resources
    Stockpile Stock;

    // What it has in the bank
    long long Money;
};

#endif
