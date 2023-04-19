#include "Country.h"
#include "Diplomacy.h"
#include "AI.h"

Country::Country(std::string Tag, std::string Name, const Stockpile& sp, bool isPlayerControlled, Color rgb) : Country(Tag, Name, sp, rgb) {
    isPlayer = isPlayerControlled;
}

Country::Country(std::string Tag, std::string Name, const Stockpile& sp, Color rgb) : color{rgb}, name{Name}, tag{Tag}, Stock{sp} {
    population = 0;
    stateCount = 0;
    isPlayer = false;

    policy = {.TaxRate = 50, .Healthcare =  30};

    technology = {  .FactoryInput = 1.0f,
                    .FactoryThroughput = 1.0f,
                    .FactoryOutput = 1.0f,
                    .MineralOutput = 1.0f,
                    .FarmOutput = 1.0f,
                    .WoodOutput = 1.0f};
}

void Country::AddState(State* state){
    ownedStates[state->State_Name] = state;
    population += int(state->State_Population);
    stateCount++;
}

void Country::RemoveState(State* state){
    if (ownedStates.contains(state->State_Name)) {
        ownedStates.erase(state->State_Name);
        stateCount--;
    }
}

void Country::Tick(){
    for (auto& [Name, state] : ownedStates) {
        state->Tick(policy.TaxRate, policy.Healthcare);
    }

    Stock.Money += int(population * 0.004 * policy.TaxRate / 100);
    Stock.Money -= int(population * 0.001 * policy.Healthcare / 100);

    HandleDiplomaticRequests();
}

void Country::AddRequest(Request request) {
    requests.push_back(request);
}

std::string Country::GetName() const {
    return name;
}

std::string Country::GetTag() const {
    return tag;
}

int Country::GetPopulation() const {
    return population;
}

bool Country::GetIfIsPlayer() const {
    return isPlayer;
}

void Country::HandleDiplomaticRequests() {
    if (isPlayer) {

    } else {
        for (auto req : requests) {
            AI::HandleRequest(req);
        }
    }

    requests.clear();
}
