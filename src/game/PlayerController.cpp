#include "game/PlayerController.h"

#include "core/Color.h"
#include "core/MainWindow.h"

#include <SDL3_image/SDL_image.h>

#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace {
struct Line {
    std::string str;

    operator std::string const&() const { return str; }

    friend std::istream& operator>>(std::istream& is, Line& l) { return std::getline(is, l.str); }
};

/*One state's daily output of the raw resources, in the order Stockpile.h lists
them. A line that names fewer than all of them leaves the rest at zero.*/
struct RawResources {
    std::array<short int, RawGoodCount> amounts{};

    operator std::array<short int, RawGoodCount> const&() const { return amounts; }

    friend std::istream& operator>>(std::istream& is, RawResources& r) {
        if(std::string line; std::getline(is, line)) {
            std::istringstream iss(std::move(line));
            for(auto& amount : r.amounts) {
                iss >> amount;
            }
        } else {
            is.setstate(std::ios::failbit);
        }
        return is;
    }
};

template<class T, class Adapter = T>
std::vector<T> LoadFromFile(const char* filename) {
    std::ifstream is(filename);
    return {std::istream_iterator<Adapter>(is), std::istream_iterator<Adapter>{}};
}
} // namespace

/*One country's opening position: an amount of every good, in the order
PerGood.h lists them, and then the money it has in the bank.*/
struct StartingBalance {
    Stockpile stock;
    long long money = 0;

    friend std::istream& operator>>(std::istream& is, StartingBalance& b) {
        if(std::string line; std::getline(is, line)) {
            std::istringstream iss(std::move(line));
            for(auto good : GoodMembers<int>) {
                iss >> b.stock.*good;
            }
            if(not(iss >> b.money)) {
                is.setstate(std::ios::failbit);
            }
        } else {
            is.setstate(std::ios::failbit);
        }
        return is;
    }
};

PlayerController::PlayerController(MainWindow& mw, const char* tag) : main_window(&mw) {
    /*Decode the map assets while the game data files are being read. The
    threads are scoped so that they are joined before UploadAssets runs -
    the GPU upload has to happen on this thread.*/
    {
        auto MapThread = std::jthread(&PlayerController::LoadMap, this);
        auto AssetsThread = std::jthread(&PlayerController::LoadUtilityAssets, this);

        LoadGameData(tag);
    }

    UploadAssets();
}

void PlayerController::LoadGameData(const char* tag) {
    // Load the Countries' Names
    auto countryNames = LoadFromFile<std::string, Line>("map/Countries/CountryNames.txt");

    // Load the country tags
    auto tags = LoadFromFile<std::string, Line>("map/Countries/CountryTags.txt");

    // Load the countries' budget at the start of the game
    auto balance = LoadFromFile<StartingBalance>("map/Countries/CountryStockpiles.txt");

    if(countryNames.size() != tags.size() || countryNames.size() != balance.size()) {
        std::cerr << "Country data mismatch " << countryNames.size() << ',' << tags.size() << ',' << balance.size() << std::endl;
        std::terminate();
    }

    // Load all state names
    auto stateNames = LoadFromFile<std::string, Line>("map/States/StateNames.txt");

    // Load all state owner tags
    auto owners = LoadFromFile<std::string, Line>("map/States/StateOwners.txt");

    // Load all the states' unique color IDs
    auto colors = LoadFromFile<Color>("map/States/StateColors.txt");

    // Load all the state's coordinates
    auto coords = LoadFromFile<Coordinate>("map/States/StateCoordinates.txt");

    // Load all the state's populations
    auto populations = LoadFromFile<int>("map/States/StatePopulations.txt");

    // Load what each state pulls out of the ground in a day
    auto resources = LoadFromFile<std::array<short int, RawGoodCount>, RawResources>("map/States/StateResources.txt");

    if(stateNames.size() != owners.size() || stateNames.size() != colors.size() || stateNames.size() != coords.size() || stateNames.size() != populations.size() || stateNames.size() != resources.size()) {
        std::cerr << "State data mismatch " << stateNames.size() << ',' << owners.size() << ',' << colors.size() << ',' << coords.size() << ',' << populations.size() << ',' << resources.size() << std::endl;
        std::terminate();
    }

    // Create all countries
    InitializeCountries(countryNames, tags, tag, balance);

    // Create all the states
    InitializeStates(owners, stateNames, coords, populations, colors, resources);

    // Initialize the date
    Date = {.Year = 1910, .Month = 1, .Day = 1, .Speed = 1, .MonthDays = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
}

void PlayerController::LoadMap() {
    map = SDL_Surface_ctx::IMG_Load("map/1910.png");
}

void PlayerController::LoadUtilityAssets() {
    provinces = SDL_Surface_ctx::IMG_Load("map/provinces.bmp");
}

void PlayerController::UploadAssets() {
    txt = SDL_Texture_ctx(*main_window, map);

    // ~48 MB of decoded pixels, dead weight once the GPU has its own copy
    map = SDL_Surface_ctx{};
}

void PlayerController::InitializeCountries(std::vector<std::string>& names, std::vector<std::string>& tags, const char* tag, const std::vector<StartingBalance>& balance) {
    for(unsigned x = 0; x < tags.size(); x++) {
        Countries.push_back(std::make_unique<Country>(tags[x], names[x], balance[x].stock, balance[x].money));
        if(tag == tags[x]) {
            player = Countries.back().get();
        }
    }

    if(player == nullptr) {
        std::cerr << "No country matches the player tag " << tag << std::endl;
        std::terminate();
    }

    for(unsigned c1 = 0; c1 < Countries.size(); c1++) {
        for(unsigned c2 = c1 + 1; c2 < Countries.size(); c2++) {
            diplo.relations.emplace(CountryPair{Countries[c1].get(), Countries[c2].get()}, Relation{100});
        }
    }
}

void PlayerController::InitializeStates(std::vector<std::string>& owners, std::vector<std::string>& names, std::vector<Coordinate>& coords, const std::vector<int>& populations, std::vector<Color>& colors,
                                        const std::vector<std::array<short int, RawGoodCount>>& resources) {
    unsigned target = 0;

    StatesArr.reserve(populations.size());
    for(unsigned x = 0; x < owners.size(); ++x) {
        for(unsigned y = 0; y < Countries.size(); y++) {
            if(owners[x] == Countries[y]->GetTag()) {
                target = y;
                break;
            }
        }

        StatesArr.emplace_back(names[x], x + 1, owners[x], owners[x], populations[x], coords[x], colors[x], resources[x], &Countries.at(target)->Stock);

        StatesMap.insert(std::pair(StatesArr.back().color.toString(), &StatesArr.back()));

        Countries.at(target)->AddState(&StatesArr.back());
    }
}

Uint32 PlayerController::MillisecondsPerDay() const {
    // Speed runs from 1 (slowest) to 4: 2000 ms per in-game day down to 200 ms
    return 10u * static_cast<Uint32>(260 - Date.Speed * 60);
}

void PlayerController::AdvanceOneDay() {
    Date.Day++;

    if(Date.Day <= Date.MonthDays[Date.Month - 1]) {
        return;
    }

    // The month has rolled over
    Date.Day = 1;
    Date.Month++;

    if(Date.Month <= 12) {
        return;
    }

    // And so has the year
    Date.Month = 1;
    Date.Year++;

    // February gains a day on a leap year
    Date.MonthDays[1] = (Date.Year % 4 == 0) ? 29 : 28;
}

void PlayerController::Update(Uint32 elapsedMs) {
    if(bIsPaused) {
        return;
    }

    const Uint32 msPerDay = MillisecondsPerDay();

    dayAccumulator += elapsedMs;

    // Discard anything beyond the catch-up budget rather than tick it all at once
    if(dayAccumulator > msPerDay * MaxCatchUpDays) {
        dayAccumulator = msPerDay * MaxCatchUpDays;
    }

    while(dayAccumulator >= msPerDay) {
        dayAccumulator -= msPerDay;

        AdvanceOneDay();
        Tick();
    }
}

void PlayerController::Pause() {
    /*Pause or unpause the game when this function is executed.
    It will always change the state of bIsPaused and set it to
    the opposite value.*/
    bIsPaused = not bIsPaused;

    // Drop the part-elapsed day so that unpausing does not tick immediately
    dayAccumulator = 0;
}

void PlayerController::ChangeSpeed(bool change) {
    // If change is true then we increase the speed and
    // if it is false it decreases the speed
    if(change && Date.Speed < 4) {
        Date.Speed++;
    } else if(!change && Date.Speed > 1) {
        Date.Speed--;
    }
}

void PlayerController::Tick() {
    // Execute the tick function for all countries (and all states)
    for(auto& i : Countries) {
        i->Tick();
    }
}
