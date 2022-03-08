#ifndef COUNTRY_H
#define COUNTRY_H

#include <vector>
#include <string>
#include "State.h"
#include "Stockpile.h"

class Country {
public:
    //Functions

    //Constructor
    Country(std::string tag, int r, int g, int b, int Res[31]);

    void AddState(State* state);

    void RemoveState(State* state);

    //This is the representing the pass of a single day
    void Tick();
public:
    //Variables
    std::string countrytag;
    std::vector<State*> OwnedStates;
    int Country_State_Count;
    int Country_Population;

    struct {
        float FactoryInput;
        float FactoryThroughput;
        float FactoryOutput;
        float MineralOutput;
        float FarmOutput;
        float WoodOutput;
    } Technology;

    struct {
        int red;
        int green;
        int blue;
    } Color;

    struct {
        int TaxRate;
    } Policy;

    //A country's currently stockpiled resources
    Stockpile Stock;
};

#endif