#ifndef STATE_H
#define STATE_H

#include <string>
#include "Factory.h"
#include "Stockpile.h"

typedef struct Color {
    short int r;
    short int g;
    short int b;
} Color;

class State {
public:
    //Constructor
    State(std::string name, int ID, std::string owner, std::string controller, int pop, short int Coords[2], short int C[3], short int Res[8], Stockpile* stock);

    //This is the representing the pass of a single day
    void Tick(int TaxRate, int HealthCare);

    void ChangeController(std::string NewOwner, Stockpile* NewStock);

    int AddFactory(Factory* NewFactory);

    int RemoveFactory(int index);

    //This is the state's name
    std::string State_Name;

    //This is the state's unique ID
    int State_ID;

    //This is the state's owner tag, expressed in three(3) letters
    std::string State_Owner;

    //This is the state's current controller tag, expressed in three(3) letters
    std::string State_Controller;
    
    //This is the state's population
    int State_Population;
    
    //This is the state's coorditates on the map expressed in pixels
    int State_Coords[2];

    //This is the state's factories
    Factory* State_Factories[4]= { nullptr, nullptr, nullptr, nullptr };

    Stockpile* TargetStockpile;

    //This is the state's unique color
    Color color;

    //This is the resources contained within one state
    struct {
        short int Coal;
        short int Oil;
        short int Timber;
        short int Rubber;
        short int Cotton;
        short int Iron;
        short int Grain;
        short int Fruit;
    } Resources;

    //These are the resources that the population of the state requires in order to maintain a good standard of living
    struct {
        short int Telephones;
        short int Fuel;
        short int Paper;
        short int Canned_food;
        short int Furniture;
        short int Clothes;
        short int Automobiles;
        short int Cement;
        short int Glass;
        short int Lumber;
        short int Fruit;
    } Pop_needs;
};
#endif