#ifndef COUNTRY_H
#define COUNTRY_H

#include <unordered_map>
#include <string>
#include <vector>
#include "State.h"
#include "Stockpile.h"

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
    //Functions

    //Constructor
    Country(std::string tag, std::string name, const Stockpile& sp, bool isPlayerControlled, Color rgb);
    Country(std::string tag, std::string name, const Stockpile& sp, Color rgb = Color{});

    //Handle the states of the country
    void AddState(State* state);
    void RemoveState(State* state);

    //This is the representing the pass of a single day
    void Tick();

    void AddRequest(Request request);

    //Accessor Functions
    std::string GetName() const;
    std::string GetTag() const;
    int GetPopulation() const;
    bool GetIfIsPlayer() const;

private:
    Color color;

    //Basic Properties
    bool isPlayer;
    std::string name;
    std::string tag;
    int population;

    int stateCount;
    std::unordered_map<std::string, State*> ownedStates;

    Technology technology;

    //The diplomatic requests that a country receives
    std::vector<Request> requests;

    void HandleDiplomaticRequests();

public:
    Policy policy;

    //A country's currently stockpiled resources
    Stockpile Stock;
};

#endif
