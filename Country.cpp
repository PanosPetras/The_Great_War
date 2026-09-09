#include "Country.h"
#include "AI.h"
#include "Diplomacy.h"

#include <algorithm>
#include <cstdint>

Country::Country(std::string Tag, std::string Name, const Stockpile& sp, long long money, bool isPlayerControlled, Color rgb) : Country(Tag, Name, sp, money, rgb) {
    isPlayer = isPlayerControlled;
}

Country::Country(std::string Tag, std::string Name, const Stockpile& sp, long long money, Color rgb) : color{rgb}, name{Name}, tag{Tag}, Stock{sp}, Money{money} {
    population = 0;
    stateCount = 0;
    isPlayer = false;

    policy = {.TaxRate = 50, .Healthcare = 30};

    technology = {.FactoryInput = 1.0f, .FactoryThroughput = 1.0f, .FactoryOutput = 1.0f, .MineralOutput = 1.0f, .FarmOutput = 1.0f, .WoodOutput = 1.0f};
}

void Country::AddState(State* state) {
    ownedStates[state->State_Name] = state;
    population += int(state->State_Population);
    stateCount++;
}

void Country::RemoveState(State* state) {
    if(ownedStates.contains(state->State_Name)) {
        ownedStates.erase(state->State_Name);
        stateCount--;
    }
}

void Country::Tick() {
    /*The states first, so that what came out of the ground today is in the
    warehouses before the factories are given the chance to eat it.*/
    for(auto& [Name, state] : ownedStates) {
        state->Tick(policy.TaxRate, policy.Healthcare);
    }

    RunFactories();

    Money += int(population * 0.004 * policy.TaxRate / 100);
    Money -= int(population * 0.001 * policy.Healthcare / 100);

    HandleDiplomaticRequests();
}

void Country::RunFactories() {
    // What a full day of work in every factory would eat
    Stockpile wanted{};
    for(auto& [Name, state] : ownedStates) {
        for(const auto& factory : state->State_Factories) {
            if(factory != nullptr) {
                wanted += factory->Consumption();
            }
        }
    }

    /*How far each good goes round everything that wants it, in thousandths.
    A good there is enough of, and a good nobody asked for, both come out at
    full.*/
    PerGood<int> share{};
    for(auto good : AllGoods) {
        const std::int64_t held = std::max(0, Stock[good]);
        share[good] = wanted[good] > 0 ? int(std::min<std::int64_t>(FullThroughput, FullThroughput * held / wanted[good])) : FullThroughput;
    }

    /*Now the day's work. Every share was worked out before a single good was
    taken, so it makes no difference which state is reached first - which
    matters, because the states are walked in whatever order the map holds
    them.*/
    for(auto& [Name, state] : ownedStates) {
        for(auto& factory : state->State_Factories) {
            if(factory != nullptr) {
                factory->Work(factory->Throughput(share), Stock);
            }
        }
    }
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
    if(isPlayer) {
    } else {
        for(auto req : requests) {
            AI::HandleRequest(req);
        }
    }

    requests.clear();
}
