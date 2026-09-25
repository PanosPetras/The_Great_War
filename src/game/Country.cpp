#include "game/Country.h"
#include "game/AI.h"
#include "game/Diplomacy.h"
#include "game/PopNeeds.h"

#include <algorithm>
#include <cstdint>
#include <utility>

Country::Country(std::string Tag, std::string Name, const Stockpile& sp, long long money, bool isPlayerControlled, Color rgb) : Country(Tag, Name, sp, money, rgb) {
    isPlayer = isPlayerControlled;
}

Country::Country(std::string Tag, std::string Name, const Stockpile& sp, long long money, Color rgb) : color{rgb}, name{std::move(Name)}, tag{std::move(Tag)}, Stock{sp}, Money{money} {
    isPlayer = false;
    satisfaction = SubsistenceSatisfaction;

    policy = {.TaxRate = 50, .Healthcare = 30};
}

void Country::AddState(State* state) {
    if(std::ranges::find(ownedStates, state) == ownedStates.end()) {
        ownedStates.push_back(state);
    }
}

void Country::RemoveState(State* state) {
    std::erase(ownedStates, state);
}

void Country::CedeState(State* state, Country& to) {
    RemoveState(state);
    to.AddState(state);
    state->ChangeController(to.GetTag(), &to.Stock);
}

void Country::Tick() {
    /*The states first, so that what came out of the ground today is in the
    warehouses before the factories are given the chance to eat it.*/
    const int growth = GrowthPermille(satisfaction);
    for(auto* state : ownedStates) {
        state->Tick(policy.TaxRate, policy.Healthcare, technology, growth);
    }

    RunFactories();

    FeedPopulation();

    CollectTaxes();

    Money += budget.Net();

    HandleDiplomaticRequests();
}

void Country::RunFactories() {
    // What a full day of work in every factory would eat
    Stockpile wanted{};
    for(auto* state : ownedStates) {
        for(const auto& factory : state->State_Factories) {
            if(factory != nullptr) {
                wanted += factory->Consumption(technology);
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
    matters, because the states are walked in whatever order the list holds
    them.*/
    budget.factories = 0;
    for(auto* state : ownedStates) {
        for(auto& factory : state->State_Factories) {
            if(factory != nullptr) {
                const int throughput = factory->Throughput(share);
                factory->Work(throughput, Stock, technology);
                budget.factories += factory->RunningCost(throughput);
            }
        }
    }
}

void Country::CollectTaxes() {
    const long long population = GetPopulation();

    /*A thousandth of a unit a head at full tax from people living at 40%,
    two and a half times that from people who want for nothing.*/
    budget.taxes = population * 25 * policy.TaxRate * satisfaction / (100 * 10'000'000LL);
    budget.healthcare = population * policy.Healthcare / (100 * 1000LL);
}

void Country::FeedPopulation() {
    const long long population = GetPopulation();

    int met = 0;
    int weights = 0;
    for(const auto& [good, perTenMillion, weight] : PopNeeds) {
        /*Rounded up, so that a small country still wants a car now and then
        rather than having every need it is too small for count as met.*/
        const long long wanted = (population * perTenMillion + TenMillion - 1) / TenMillion;
        if(wanted <= 0) continue;

        const long long taken = std::min<long long>(std::max(0, Stock[good]), wanted);
        Stock[good] -= int(taken);

        met += int(FullThroughput * taken / wanted) * weight;
        weights += weight;
    }

    satisfaction = weights > 0 ? met / weights : FullThroughput;
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
    double population = 0;
    for(const auto* state : ownedStates) {
        population += state->State_Population;
    }
    return int(population);
}

int Country::GetSatisfaction() const {
    return satisfaction;
}

const Budget& Country::GetBudget() const {
    return budget;
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
