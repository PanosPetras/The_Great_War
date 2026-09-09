#ifndef STATE_H
#define STATE_H

#include "core/Color.h"
#include "core/Coordinate.h"
#include "game/Factory.h"
#include "game/Stockpile.h"

#include <array>
#include <memory>
#include <string>

class State {
public:
    // Constructor
    State(std::string name, int ID, std::string owner, std::string controller, int pop, Coordinate Coords, Color C, const std::array<short int, RawGoodCount>& Res, Stockpile* stock);

    /*A single day for the state itself: the population grows and the mines,
    fields and forests hand over the day's output. The state's factories are
    not run from here - they draw on a stockpile that is shared with every
    other factory in the country, so the country runs them all together.*/
    void Tick(int TaxRate, int HealthCare);

    void ChangeController(std::string NewOwner, Stockpile* NewStock);

    int AddFactory(std::unique_ptr<Factory>& NewFactory);

    int RemoveFactory(unsigned index);

    // This is the state's name
    std::string State_Name;

    // This is the state's unique ID
    int State_ID;

    // This is the state's owner tag, expressed in three(3) letters
    std::string State_Owner;

    // This is the state's current controller tag, expressed in three(3) letters
    std::string State_Controller;

    // This is the state's population
    double State_Population;

    // This is the state's coorditates on the map expressed in pixels
    Coordinate State_Coords;

    // This is the state's factories
    std::array<std::unique_ptr<Factory>, 4> State_Factories = {nullptr, nullptr, nullptr, nullptr};

    Stockpile* TargetStockpile;

    // This is the state's unique color
    Color color;

    /*What the state produces each day, indexed by Good. Only raw resources come
    out of the ground, and those are the leading run of the enum, so the array
    stops there rather than carrying a slot for every manufactured good.*/
    std::array<short int, RawGoodCount> Resources{};

    /*What the state's population wants each day to keep a good standard of
    living. A population runs to millions, which a short int cannot hold.*/
    PerGood<int> Pop_needs;
};
#endif
