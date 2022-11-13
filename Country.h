#ifndef COUNTRY_H
#define COUNTRY_H

#include <unordered_map>
#include <string>
#include "State.h"
#include "Stockpile.h"

class Country;

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
    int ExportRate;
    int Healthcare;
};

struct War {
    std::vector<Country*> Side1;
    std::vector<Country*> Side2;
    int Score;
};

struct TradeDeal {
    float Offset;
    Country* Signatories[2];
};

struct Diplomacy {
    std::vector<War> Wars;
    std::vector<Country*> Alliances;
    std::vector<TradeDeal> TradeDeals;
};

class Country {
public:
    //Functions

    //Constructor
    Country(std::string tag, int r, int g, int b, int Res[31]);

    //Destructor
    ~Country();

    void AddState(State* state);

    void RemoveState(State* state);

    //This is the representing the pass of a single day
    void Tick();

    //Variables
    std::string countrytag;
    std::unordered_map<std::string, State*>* OwnedStates;
    int Country_State_Count;
    int Country_Population;
    
    Technology technology;
    
    Color color;

    Policy policy;

    //A country's currently stockpiled resources
    Stockpile Stock;
};

#endif