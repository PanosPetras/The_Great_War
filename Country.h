#ifndef COUNTRY_H
#define COUNTRY_H

#include <unordered_map>
#include <string>
#include "State.h"
#include "Stockpile.h"

struct Technology {
    float FactoryInput;
    float FactoryThroughput;
    float FactoryOutput;
    float MineralOutput;
    float FarmOutput;
    float WoodOutput;
};

struct Policy {
    int TaxRate;
    int Healthcare;
};

class Country {
public:
    //Functions

    //Constructor
    Country(std::string tag, std::string name, int r, int g, int b, int Res[31], bool isPlayerControlled);
    Country(std::string tag, std::string name, int r, int g, int b, int Res[31]);

    //Destructor
    ~Country();

    void AddState(State* state);

    void RemoveState(State* state);

    //This is the representing the pass of a single day
    void Tick();

    //Accessor Functions
    std::string GetName() const;
    std::string GetTag() const;
    int GetPopulation() const;

private:
    Color color;

    //Basic Properties
    bool isPlayer;
    std::string name;
    std::string tag;
    int population;

    int stateCount;
    std::unordered_map<std::string, State*>* ownedStates;
    
    Technology technology;

public:
    Policy policy;

    //A country's currently stockpiled resources
    Stockpile Stock;
};

#endif