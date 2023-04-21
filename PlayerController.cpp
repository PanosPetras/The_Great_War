#include "PlayerController.h"

#include "Color.h"
#include "MainWindow.h"

#include <SDL_image.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>
#include <vector>

namespace {
struct Line {
    std::string str;

    operator std::string const&() const { return str; }

    friend std::istream& operator>>(std::istream& is, Line& l) { return std::getline(is, l.str); }
};

template<class T, class Adapter = T>
std::vector<T> LoadFromFile(const char* filename) {
    std::ifstream is(filename);
    return {std::istream_iterator<Adapter>(is), std::istream_iterator<Adapter>{}};
}
} // namespace

PlayerController::PlayerController(MainWindow& mw, const char* tag) : main_window(&mw), player_tag(tag) {
    // Create the map and assets in separate threads
    auto MapThread = std::jthread(&PlayerController::LoadMap, this);
    auto AssetsThread = std::jthread(&PlayerController::LoadUtilityAssets, this);

    // Load the Countries' Names
    auto countryNames = LoadFromFile<std::string, Line>("map/Countries/CountryNames.txt");

    // Load the country tags
    auto tags = LoadFromFile<std::string, Line>("map/Countries/CountryTags.txt");

    // Load the countries' budget at the start of the game
    auto balance = LoadFromFile<Stockpile>("map/Countries/CountryStockpiles.txt");

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

    if(stateNames.size() != owners.size() || stateNames.size() != colors.size() || stateNames.size() != coords.size() || stateNames.size() != populations.size()) {
        std::cerr << "State data mismatch " << stateNames.size() << ',' << owners.size() << ',' << colors.size() << ',' << coords.size() << ',' << populations.size() << std::endl;
        std::terminate();
    }

    // Create all countries
    InitializeCountries(countryNames, tags, tag, balance);

    // Create all the states
    InitializeStates(owners, stateNames, coords, populations, colors);

    // Initialize the date
    Date = {.Year = 1910, .Month = 1, .Day = 1, .Speed = 1, .MonthDays = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
}

PlayerController::~PlayerController() {
    // Stop the date thread if the game is not paused
    if(bIsPaused == false) {
        Pause();
    }
}

void PlayerController::LoadMap() {
    map = SDL_Surface_ctx::IMG_Load("map/1910.png");
    auto base = SDL_Surface_ctx::CreateRGBSurface(0, 16383, 2160, 32, 0, 0, 0, 0);

    SDL_Rect strect = {.x = 232, .y = 0, .w = 5616, .h = 2160};
    SDL_BlitSurface(map, &strect, base, nullptr);
    strect = {.x = -5616 + 232, .y = 0, .w = 5616 * 2, .h = 2160};
    SDL_BlitSurface(map, &strect, base, nullptr);
    strect = {.x = -5616 * 2 + 232, .y = 0, .w = 5616 * 3, .h = 2160};
    SDL_BlitSurface(map, &strect, base, nullptr);

    txt = SDL_Texture_ctx(*main_window, base);
}

void PlayerController::LoadUtilityAssets() {
    provinces = SDL_Surface_ctx::IMG_Load("map/provinces.bmp");

    auto base = SDL_Surface_ctx::CreateRGBSurface(0, 16383, 2160, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
    overlay = SDL_Texture_ctx(*main_window, base);
}

void PlayerController::InitializeCountries(std::vector<std::string>& names, std::vector<std::string>& tags, const char* tag, const std::vector<Stockpile>& balance) {
    for(unsigned x = 0; x < tags.size(); x++) {
        CountriesArr.push_back(std::make_unique<Country>(tags[x], names[x], balance[x]));
        if(tag == tags[x]) {
            player_index = x;
        }
    }

    for(unsigned c1 = 0; c1 < CountriesArr.size(); c1++) {
        for(unsigned c2 = c1 + 1; c2 < CountriesArr.size(); c2++) {
            diplo.relations.emplace(CountryPair{CountriesArr[c1].get(), CountriesArr[c2].get()}, Relation{100});
        }
    }
}

void PlayerController::InitializeStates(std::vector<std::string>& owners, std::vector<std::string>& names, std::vector<Coordinate>& coords, const std::vector<int>& populations, std::vector<Color>& colors) {
    short int res[8] = {50, 50, 50, 50, 50, 50, 50, 50};
    unsigned target = 0;

    StatesArr.reserve(populations.size());
    for(unsigned x = 0; x < owners.size(); ++x) {
        for(unsigned y = 0; y < CountriesArr.size(); y++) {
            if(owners[x] == CountriesArr[y]->GetTag()) {
                target = y;
                break;
            }
        }

        StatesArr.emplace_back(names[x], x + 1, owners[x], owners[x], populations[x], coords[x], colors[x], res, &CountriesArr.at(target)->Stock);

        StatesMap.insert(std::pair(StatesArr.back().color.toString(), &StatesArr.back()));

        CountriesArr.at(target)->AddState(&StatesArr.back());
    }
}

void PlayerController::AdvanceDate() {
    while(bIsPaused == false) {
        for(int i = 0; i < 10 && bIsPaused == false; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(260 - Date.Speed * 60));
        }

        // Stop if the game is paused
        if(bIsPaused) {
            return;
        }

        // Advance by one day
        Date.Day++;

        // Execute the tick function
        Tick();

        // Check whether the month has changed
        if(Date.Day == Date.MonthDays[Date.Month - 1] + 1) {
            Date.Month++;
            Date.Day = 1;

            // Check whether the year has changed
            if(Date.Month == 13) {
                Date.Year++;
                Date.Month = 1;

                // Check whether the current year is a leap year
                if(Date.Year % 4 == 0) {
                    Date.MonthDays[1] = 29;
                } else {
                    Date.MonthDays[1] = 28;
                }
            }
        }
    }
}

void PlayerController::Pause() {
    /*Pause or unpause the game when this function is executed.
    It will always change the state of the Date.bIsPaused and
    set it to the opposite value.*/

    if(bIsPaused) {
        bIsPaused = false;
        // Create a new thread that will be running the AdvanceDate function in parallel with everything else
        std::cerr << "PlayerController::Pause: Starting thread\n";
        thread = std::jthread(&PlayerController::AdvanceDate, this);
    } else {
        bIsPaused = true;
        // Wait until the date thread has stopped execution
        std::cerr << "PlayerController::Pause: Waiting for thread\n";
        thread.join();
        std::cerr << "PlayerController::Pause: Thread done\n";
    }
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
    for(auto& i : CountriesArr) {
        i->Tick();
    }
}
