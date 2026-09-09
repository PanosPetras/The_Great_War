#include "State.h"

State::State(std::string name, int ID, std::string owner, std::string controller, int pop, Coordinate Coords, Color C, const std::array<short int, RawGoodCount>& Res, Stockpile* stock) :
    State_Name{std::move(name)}, State_ID{ID}, State_Owner{std::move(owner)}, State_Controller{std::move(controller)}, State_Population{static_cast<double>(pop)}, State_Coords{Coords}, color{C} {
    // These variables might change during the game's flow
    Pop_needs = {.Fruit = int(pop * 0.024),
                 .Glass = int(pop * 0.14),
                 .Lumber = int(pop * 0.1),
                 .Cement = int(pop * 0.06),
                 .Canned_food = int(pop * 0.4),
                 .Furniture = int(pop * 0.08),
                 .Clothes = int(pop * 0.12),
                 .Automobiles = int(pop * 0.05),
                 .Telephones = int(pop * 0.04),
                 .Fuel = int(pop * 0.02),
                 .Paper = int(pop * 0.15)};

    Resources = Res;

    TargetStockpile = stock;
}

void State::Tick(int TaxRate, int HealthCare) {
    State_Population += State_Population * (0.00005479452 / (1.0 + TaxRate / 200.0)) * (1 + HealthCare / 160.0);

    for(std::size_t good = 0; good < Resources.size(); ++good) {
        (*TargetStockpile)[Good(good)] += Resources[good];
    }

    for(unsigned x = 0; x < 4; x++) {
        if(State_Factories[x] != nullptr) {
            State_Factories[x]->Tick();
        }
    }
}

void State::ChangeController(std::string NewOwner, Stockpile* NewStock) {
    State_Controller = NewOwner;
    TargetStockpile = NewStock;
    for(unsigned x = 0; x < 4; x++) {
        if(State_Factories[x] != nullptr) {
            State_Factories[x]->ChangeOwner(NewStock);
        }
    }
}

int State::AddFactory(std::unique_ptr<Factory>& NewFactory) {
    for(unsigned x = 0; x < 4; x++) {
        if(State_Factories[x] == nullptr) {
            State_Factories[x] = std::move(NewFactory);
            return 0;
        }
    }
    return -1;
}

int State::RemoveFactory(unsigned index) {
    if(State_Factories[index]) {
        State_Factories[index] = nullptr;

        return 0;
    }
    return -1;
}
