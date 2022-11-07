#include "PlayerController.h"
#include "SDL_ColorDetection.h"
#include <vector>
#include <future>
#include <fstream>
#include <string>
#include <SDL_image.h>
#include <SDL_thread.h>

PlayerController::PlayerController(SDL_Renderer* r, const char* tag) {
	//Save a reference to the window's renderer
	RendererReference = r;

	//Save the player's country tag
	player_tag = tag;

	//Create the map on a separate thread
	SDL_Thread* MapThread = SDL_CreateThread(&PlayerController::LoadMap, NULL, this);
	SDL_Thread* AssetsThread = SDL_CreateThread(&PlayerController::LoadUtilityAssets, NULL, this);

	//Allocate space for some members
	StatesArr = new State* [2703];
	//StatesMap = new std::unordered_map<std::string, State*>;

	//Create the variables needed to load all the needed data
	std::string line;
	std::ifstream myfile("map/Countries/CountryTags.txt");

	//Load the country tags
	auto tags = LoadCountryTags(myfile);

	//Load all state names
	myfile.open("map/States/StateNames.txt");
	auto Names = LoadStateNames(myfile);

	//Load all state owner tags
	myfile.open("map/States/StateOwners.txt");
	auto owners = LoadStateOwnerTags(myfile);

	//Load all the states' unique color IDs
	myfile.open("map/States/StateColors.txt");
	auto colors = LoadStateColors(myfile);

	//Load all the state's coordinates
	myfile.open("map/States/StateCoordinates.txt");
	auto coords = LoadStateCoordinates(myfile);

	//Create all countries
	InitializeCountries(tags, tag);

	//Create all the states
	InitializeStates(owners, Names, coords, colors);

	//Free allocated memory
	delete[] colors, coords;

	//Initialize the date
	Date = { .Year = 1910, .Month = 1, .Day = 1, .Speed = 1, .bIsPaused = true, .MonthDays = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} };

	//Wait for the map to be loaded
	SDL_WaitThread(AssetsThread, nullptr);
	SDL_WaitThread(MapThread, nullptr);
}

PlayerController::~PlayerController() {
	//Delete all objects created by the player Controller
	for (int x = 0; x < 2703; x++) {
		delete StatesArr[x];
	}
	for (int x = 0; x < 58; x++) {
		delete CountriesArr[x];
	}

	//Free all allocated Memory
	delete StatesArr;
	//delete StatesMap;

	//Stop the date thread if the game is not paused
	if (Date.bIsPaused == false) {
		Pause();
	}

	//Remove the map surface and texture from memory
	SDL_DestroyTexture(txt);
	SDL_DestroyTexture(overlay);
	SDL_FreeSurface(map);
	SDL_FreeSurface(provinces);
}

int PlayerController::LoadMap(void* pc){
	PlayerController* PC = (PlayerController*)pc;
	PC->map = IMG_Load("map/1910.png");

	SDL_Surface* base = SDL_CreateRGBSurface(0, 16383, 2160, 32, 0, 0, 0, 0);

	SDL_Rect strect = { .x = 232, .y = 0, .w = 5616 , .h = 2160 };
	SDL_BlitSurface(PC->map, &strect, base, NULL);
	strect = { .x = -5616 + 232, .y = 0, .w = 5616 * 2 , .h = 2160 };
	SDL_BlitSurface(PC->map, &strect, base, NULL);
	strect = { .x = -5616 * 2 + 232, .y = 0, .w = 5616 * 3 , .h = 2160 };
	SDL_BlitSurface(PC->map, &strect, base, NULL);
	PC->txt = SDL_CreateTextureFromSurface(PC->RendererReference, base);
	SDL_FreeSurface(base);

	return 0;
}

int PlayerController::LoadUtilityAssets(void* pc){
	PlayerController* PC = (PlayerController*)pc;
	PC->provinces = IMG_Load("map/provinces.bmp");

	SDL_Surface* base = SDL_CreateRGBSurface(0, 16383, 2160, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	PC->overlay = SDL_CreateTextureFromSurface(PC->RendererReference, base);
	SDL_FreeSurface(base);

	return 0;
}

VectorSmartPointer PlayerController::LoadCountryTags(std::ifstream& file) {
	auto tags = std::make_unique<std::vector<std::string>>();
	std::string line;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			tags->push_back(line);
		}
		file.close();
	}

	return tags;
}

VectorSmartPointer PlayerController::LoadStateNames(std::ifstream& file) {
	auto names = std::make_unique<std::vector<std::string>>();
	std::string line;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			names->push_back(line);
		}
		file.close();
	}

	return names;
}

VectorSmartPointer PlayerController::LoadStateOwnerTags(std::ifstream& file) {
	auto owners = std::make_unique<std::vector<std::string>>();
	std::string line;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			owners->push_back(line);
		}
		file.close();
	}

	return owners;
}

unsigned char (*PlayerController::LoadStateColors(std::ifstream& file))[3] {
	auto colors = new unsigned char[2703][3]; //std::make_unique<short(*)[3]>(2703);
	std::string line;
	int x = 0;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			colors[x][0] = stoi(line.substr(0, 3));
			colors[x][1] = stoi(line.substr(4, 3));
			colors[x][2] = stoi(line.substr(8, 8));
			x++;
		}
		file.close();
	}

	return colors;
}

short (*PlayerController::LoadStateCoordinates(std::ifstream& file))[2] {
	auto coords = new short[2703][2];
	std::string line;
	int x = 0;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line == "---") {
				coords[x][0] = -1;
				coords[x][1] = -1;
			}
			else {
				coords[x][0] = std::stoi(line.substr(0, 4));
				coords[x][1] = std::stoi(line.substr(7, 11));
			}
			x++;
		}
		file.close();
	}

	return coords;
}

void PlayerController::InitializeCountries(VectorSmartPointer& tags, const char* tag){
	int Res[31] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000 };

	for (int x = 0; x < 58; x++) {
		CountriesArr[x] = new Country(tags->at(x), 0, 0, 0, Res);
		if (tag == tags->at(x)) {
			player_index = x;
		}
	}
}

void PlayerController::InitializeStates(VectorSmartPointer& owners, VectorSmartPointer& names, short(*coords)[2], unsigned char(*colors)[3]){
	short int res[8] = { 50, 50, 50, 50, 50, 50, 50, 50 };
	int target = 0;
	State* state;

	for (int x = 0; x < 2703; x++) {
		for (int y = 0; y < 58; y++) {
			if (owners->at(x) == CountriesArr[y]->countrytag) {
				target = y;
				break;
			}
		}

		state = new State(names->at(x), x + 1, owners->at(x), owners->at(x), 647428, coords[x], colors[x], res, &CountriesArr[target]->Stock);

		StatesMap.insert(std::pair(state->color.toString(), state));
		StatesArr[x] = state;

		CountriesArr[target]->AddState(state);
	}
}

int PlayerController::AdvanceDate(void* ref){
	PlayerController* reference = static_cast<PlayerController*>(ref);

	while (true)
	{
		for (int i = 0; i < 10 && reference->Date.bIsPaused == false; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(2 * 100 / reference->Date.Speed));
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
	for (int x = 0; x < 58; x++) {
		CountriesArr[x]->Tick();
	}
}