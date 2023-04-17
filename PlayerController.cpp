#include "Color.h"
#include "PlayerController.h"
#include "SDL_ColorDetection.h"

#include <SDL_image.h>
#include <SDL_thread.h>

#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>

namespace {
struct Line {
    std::string str;

    operator std::string& () { return str; }
    operator std::string const& () const { return str; }

    friend std::istream& operator>>(std::istream& is, Line& l) {
        return std::getline(is, l.str);
    }
};

template<class T, class Adapter = T>
std::vector<T> LoadFromFile(const char* filename) {
    std::ifstream is(filename);
    return {std::istream_iterator<Adapter>(is), std::istream_iterator<Adapter>{}};
}
} // namesapce

PlayerController::PlayerController(SDL_Renderer_ctx& r, const char* tag) : RendererReference(r) {
	//Save the player's country tag
	player_tag = tag;

	//Create the map on a separate thread
	SDL_Thread* MapThread = SDL_CreateThread(&PlayerController::LoadMap, nullptr, this);
	SDL_Thread* AssetsThread = SDL_CreateThread(&PlayerController::LoadUtilityAssets, nullptr, this);

	//Load the Countries' Names
	auto countryNames = LoadFromFile<std::string, Line>("map/Countries/CountryNames.txt");

	//Load the country tags
	auto tags = LoadFromFile<std::string, Line>("map/Countries/CountryTags.txt");

	//Load the countries' budget at the start of the game
	auto balance = LoadFromFile<int>("map/Countries/CountryBalances.txt");

    if(countryNames.size() != tags.size() || countryNames.size() != balance.size()) {
        std::cerr << "Country data mismatch " << countryNames.size() << ',' << tags.size() << ',' << balance.size() << std::endl;
        std::terminate();
    }

	//Load all state names
	auto stateNames = LoadFromFile<std::string, Line>("map/States/StateNames.txt");

	//Load all state owner tags
	auto owners = LoadFromFile<std::string, Line>("map/States/StateOwners.txt");

	//Load all the states' unique color IDs
	auto colors = LoadFromFile<Color>("map/States/StateColors.txt");

	//Load all the state's coordinates
	auto coords = LoadFromFile<Coordinate>("map/States/StateCoordinates.txt");

	//Load all the state's populations
	auto populations = LoadFromFile<int>("map/States/StatePopulations.txt");

    if(stateNames.size() != owners.size() || stateNames.size() != colors.size() || stateNames.size() != coords.size() || stateNames.size() != populations.size()) {
        std::cerr << "State data mismatch " << stateNames.size() << ',' << owners.size() << ',' << colors.size() << ',' << coords.size() << ',' << populations.size() << std::endl;
        std::terminate();
    }

	//Create all countries
	InitializeCountries(countryNames, tags, tag, balance);

	//Create all the states
	InitializeStates(owners, stateNames, coords, populations, colors);

	//Initialize the date
	Date = { .Year = 1910, .Month = 1, .Day = 1, .Speed = 1, .bIsPaused = true, .MonthDays = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} };

	//Wait for the map to be loaded
	SDL_WaitThread(AssetsThread, nullptr);
	SDL_WaitThread(MapThread, nullptr);
}

PlayerController::~PlayerController() {
	//Stop the date thread if the game is not paused
	if (Date.bIsPaused == false) {
		Pause();
	}
}

int PlayerController::LoadMap(void* pc){
	PlayerController& PC = *static_cast<PlayerController*>(pc);

	PC.map = SDL_Surface_ctx::IMG_Load("map/1910.png");
	SDL_Surface_ctx base(SDL_CreateRGBSurface(0, 16383, 2160, 32, 0, 0, 0, 0));

	SDL_Rect strect = { .x = 232, .y = 0, .w = 5616 , .h = 2160 };
	SDL_BlitSurface(PC.map, &strect, base, NULL);
	strect = { .x = -5616 + 232, .y = 0, .w = 5616 * 2 , .h = 2160 };
	SDL_BlitSurface(PC.map, &strect, base, NULL);
	strect = { .x = -5616 * 2 + 232, .y = 0, .w = 5616 * 3 , .h = 2160 };
	SDL_BlitSurface(PC.map, &strect, base, NULL);

	PC.txt = SDL_Texture_ctx(PC.RendererReference, base);

	return 0;
}

int PlayerController::LoadUtilityAssets(void* pc){
	PlayerController& PC = *static_cast<PlayerController*>(pc);

	PC.provinces = SDL_Surface_ctx::IMG_Load("map/provinces.bmp");

	SDL_Surface_ctx base(SDL_CreateRGBSurface(0, 16383, 2160, 32, 0xff, 0xff00, 0xff0000, 0xff000000));
	PC.overlay = SDL_Texture_ctx(PC.RendererReference, base);

	return 0;
}

void PlayerController::InitializeCountries(std::vector<std::string>& names, std::vector<std::string>& tags, const char* tag, std::vector<int>& balance){
	int Res[31] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000 };

	for (unsigned x = 0; x < tags.size(); x++) {
		Res[30] = balance[x];
		CountriesArr.push_back(std::make_unique<Country>(tags[x], names[x], 0, 0, 0, Res));
		if (tag == tags[x]) {
			player_index = x;
		}
	}

	for (unsigned c1 = 0; c1 < CountriesArr.size(); c1++) {
		for (unsigned c2 = c1 + 1; c2 < CountriesArr.size(); c2++) {
			diplo.relations.emplace(CountryPair{CountriesArr[c1].get(), CountriesArr[c2].get()}, Relation{100});
		}
	}
}

void PlayerController::InitializeStates(std::vector<std::string>& owners, std::vector<std::string>& names, std::vector<Coordinate>& coords, const std::vector<int>& populations, std::vector<Color>& colors){
	short int res[8] = { 50, 50, 50, 50, 50, 50, 50, 50 };
	int target = 0;

        StatesArr.reserve(populations.size());
	for (unsigned x = 0; x < owners.size(); ++x) {
		for (unsigned y = 0; y < CountriesArr.size(); y++) {
			if (owners[x] == CountriesArr[y]->GetTag()) {
				target = y;
				break;
			}
		}

                StatesArr.emplace_back(names[x], x + 1, owners[x], owners[x], populations[x], coords[x], colors[x], res, &CountriesArr.at(target)->Stock);

		StatesMap.insert(std::pair(StatesArr.back().color.toString(), &StatesArr.back()));

		CountriesArr.at(target)->AddState(&StatesArr.back());
	}
}

int PlayerController::AdvanceDate(void* ref){
	PlayerController* reference = static_cast<PlayerController*>(ref);

	while (true) {
		for (int i = 0; i < 10 && reference->Date.bIsPaused == false; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(260 - reference->Date.Speed * 60));
		}

		//Stop if the game is paused
		if (reference->Date.bIsPaused) {
			return 0;
		}

		//Advance by one day
		reference->Date.Day++;

		//Execute the tick function
		reference->Tick();

		//Check whether the month has changed
		if (reference->Date.Day == reference->Date.MonthDays[reference->Date.Month - 1] + 1) {
			reference->Date.Month++;
			reference->Date.Day = 1;

			//Check whether the year has changed
			if (reference->Date.Month == 13) {
				reference->Date.Year++;
				reference->Date.Month = 1;

				//Check whether the current year is a leap year
				if (reference->Date.Year % 4 == 0) {
					reference->Date.MonthDays[1] = 29;
				} else {
					reference->Date.MonthDays[1] = 28;
				}
			}
		}
	}
}

void PlayerController::Pause(){
	/*Pause or unpause the game when this function is executed.
	It will always change the state of the Date.bIsPaused and
	set it to the opposite value.*/
	if (Date.bIsPaused) {
		Date.bIsPaused = false;
		//Create a new thread that will be running the AdvanceDate function in parallel with everything else
		thread = SDL_CreateThread(&PlayerController::AdvanceDate, NULL, this);
	} else {
		Date.bIsPaused = true;
		//Wait until the date thread has stopped execution
		SDL_WaitThread(thread, nullptr);
	}
}

void PlayerController::ChangeSpeed(bool change){
	//If change is true then we increase the speed and
	//if it is false it decreases the speed
	if (change && Date.Speed < 4) {
		Date.Speed++;
	} else if (!change && Date.Speed > 1) {
		Date.Speed--;
	}
}

void PlayerController::Tick(){
	//Execute the tick function for all countries (and all states)
	for (auto& i : CountriesArr) {
		i->Tick();
	}
}
