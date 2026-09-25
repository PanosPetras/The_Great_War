#include "game/State.h"

#include <utility>

State::State(std::string name, int ID, std::string owner, std::string controller, int pop, Coordinate Coords, Color C, const std::array<short int, RawGoodCount>& Res, Stockpile* stock) :
    State_Name{std::move(name)}, State_ID{ID}, State_Owner{std::move(owner)}, State_Controller{std::move(controller)}, State_Population{static_cast<double>(pop)}, State_Coords{Coords}, color{C} {
    Resources = Res;

    TargetStockpile = stock;
}

void State::Tick(int TaxRate, int HealthCare, const Technology& tech, int growthPermille) {
    State_Population += State_Population * (0.00005479452 / (1.0 + TaxRate / 200.0)) * (1 + HealthCare / 160.0) * growthPermille / 1000.0;

    for(std::size_t good = 0; good < Resources.size(); ++good) {
        (*TargetStockpile)[Good(good)] += ScaleByPermille(Resources[good], tech.ExtractionOf(Good(good)));
    }
}

void State::ChangeController(std::string NewController, Stockpile* NewStock) {
    State_Controller = std::move(NewController);
    TargetStockpile = NewStock;
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
