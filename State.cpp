#include "State.h"

State::State(std::string name, int ID, std::string owner, std::string controller, int pop, Coordinate Coords, Color C, short int Res[8], Stockpile* stock) :
	State_Name{std::move(name)},
	State_ID{ID},
	State_Owner{std::move(owner)},
	State_Controller{std::move(controller)},
	State_Population{static_cast<double>(pop)},
	State_Coords{Coords},
	color{C}
{
	//These variables might change during the game's flow
	Pop_needs = {
		.Telephones = short(pop * 0.04),
		.Fuel = short(pop * 0.02),
		.Paper = short(pop * 0.15),
		.Canned_food = short(pop * 0.4),
		.Furniture = short(pop * 0.08),
		.Clothes = short(pop * 0.12),
		.Automobiles = short(pop * 0.05),
		.Cement = short(pop * 0.06),
		.Glass = short(pop * 0.14),
		.Lumber = short(pop * 0.1),
		.Fruit = short(pop * 0.024)};

	Resources = {
		.Coal = Res[0],
		.Oil = Res[1],
		.Timber = Res[2],
		.Rubber = Res[3],
		.Cotton = Res[4],
		.Iron = Res[5],
		.Grain = Res[6],
		.Fruit = Res[7]};

	TargetStockpile = stock;
}

void State::Tick(int TaxRate, int HealthCare){
    State_Population += State_Population * (0.00005479452 / (1.0 + TaxRate / 200.0)) * (1 + HealthCare / 160.0);

    TargetStockpile->Coal += Resources.Coal;
    TargetStockpile->Oil += Resources.Oil;
    TargetStockpile->Timber += Resources.Timber;
    TargetStockpile->Rubber += Resources.Rubber;
    TargetStockpile->Cotton += Resources.Cotton;
    TargetStockpile->Iron += Resources.Iron;
    TargetStockpile->Grain += Resources.Grain;
    TargetStockpile->Fruit += Resources.Fruit;

    for (unsigned x = 0; x < 4; x++) {
        if (State_Factories[x] != nullptr) {
            State_Factories[x]->Tick();
        }
    }
}

void State::ChangeController(std::string NewOwner, Stockpile* NewStock){
    State_Controller = NewOwner;
    TargetStockpile = NewStock;
    for (unsigned x = 0; x < 4; x++) {
        if (State_Factories[x] != nullptr) {
            State_Factories[x]->ChangeOwner(NewStock);
        }
    }
}

int State::AddFactory(std::unique_ptr<Factory>& NewFactory){
    for (unsigned x = 0; x < 4; x++) {
        if (State_Factories[x] == nullptr) {
            State_Factories[x] = std::move(NewFactory);
            return 0;
        }
    }
    return -1;
}

int State::RemoveFactory(unsigned index){
    if (State_Factories[index]) {
        State_Factories[index] = nullptr;

        return 0;
    }
    return -1;
}
