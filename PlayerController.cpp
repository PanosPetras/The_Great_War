#include "PlayerController.h"

PlayerController::PlayerController(SDL_Renderer* r, const char* tag) {
	RendererReference = r;

	//Save the player's country tag
	player_tag = tag;

	//Create the variables needed to load all the needed data
	std::string line;
	std::ifstream myfile("map/Countries/CountryTags.txt");

	//Load the country tags
	std::vector<std::string>* tags = new std::vector<std::string>;
	int x = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			tags->push_back(line);
		}
		myfile.close();
	}

	//Load all state names
	myfile.open("map/States/StateNames.txt");
	std::vector<std::string>* Names = new std::vector<std::string>;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			Names->push_back(line);
		}
		myfile.close();
	}

	//Load all state owner tags
	myfile.open("map/States/StateOwners.txt");
	std::vector<std::string>* owners = new std::vector<std::string>;
	x = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			owners->push_back(line);
			x++;
		}
		myfile.close();
	}

	//Load all the states' unique color IDs
	myfile.open("map/States/StateColors.txt");
	short int colori[2703][3];
	x = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			colori[x][0] = stoi(line.substr(0, 3));
			colori[x][1] = stoi(line.substr(4, 3));
			colori[x][2] = stoi(line.substr(8, 8));
			x++;
		}
		myfile.close();
	}

	//Load all the state's coordinates
	myfile.open("map/States/StateCoordinates.txt");
	short int Coords[2703][2];
	x = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line == "---") {
				Coords[x][0] = -1;
				Coords[x][1] = -1;
			}
			else {
				Coords[x][0] = stoi(line.substr(0, 4));
				Coords[x][1] = stoi(line.substr(7, 11));
			}
			x++;
		}
		myfile.close();
	}

	int Res[31] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000 };

	//Create all countries
	for (x = 0; x < 58; x++) {
		CountriesArr[x] = new Country(tags->at(x), 0, 0, 0, Res);
		if (tag == tags->at(x)) {
			player_index = x;
		}
	}

	//Create all the states
	short int res[8] = { 50, 50, 50, 50, 50, 50, 50, 50 };
	int target = 0;
	Factory* FC;
	for (x = 0; x < 2703; x++) {
		for (int y = 0; y < 58; y++) {
			if (owners->at(x) == CountriesArr[y]->countrytag) {
				target = y;
				break;
			}
		}
		StatesArr[x] = new State(Names->at(x), x + 1, owners->at(x), owners->at(x), 647428, Coords[x], colori[x], res, &CountriesArr[target]->Stock);
		CountriesArr[target]->AddState(StatesArr[x]);
	}

	//Initialize the date
	Date = { .Year = 1910, .Month = 1, .Day = 1, .Speed = 1, .bIsPaused = true, .MonthDays = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} };

	//Create the map
	map = IMG_Load("map/1910.png");

	SDL_Surface* base = SDL_CreateRGBSurface(0, 16383, 2160, 32, 0, 0, 0, 0 );

	SDL_Rect strect = { .x = 232, .y = 0, .w = 5616 , .h = 2160 };
	SDL_BlitSurface(map, &strect, base, NULL);
	strect = { .x = -5616 + 232, .y = 0, .w = 5616* 2 , .h = 2160 };
	SDL_BlitSurface(map, &strect, base, NULL);
	strect = { .x = -5616 * 2 + 232, .y = 0, .w = 5616 * 3 , .h = 2160 };
	SDL_BlitSurface(map, &strect, base, NULL);
	txt = SDL_CreateTextureFromSurface(r, base);
	SDL_FreeSurface(base);

	provinces = IMG_Load("map/provinces.bmp");
	ormap = IMG_Load("map/1910.png");

	base = SDL_CreateRGBSurface(0, 16383, 2160, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	overlay = SDL_CreateTextureFromSurface(r, base);
	SDL_FreeSurface(base);

	delete tags, Names, owners;
}

PlayerController::~PlayerController(){
	//Delete all objects created by the player Controller
	for (int x = 0; x < 2703; x++) {
		delete StatesArr[x];
	}
	for (int x = 0; x < 58; x++) {
		delete CountriesArr[x];
	}

	//Stop the date thread if the game is not paused
	if (Date.bIsPaused == false) {
		Date.bIsPaused = true;
		SDL_WaitThread(thread, nullptr);
	}

	//Remove the map surface and texture from memory
	SDL_DestroyTexture(txt);
	SDL_DestroyTexture(overlay);
	SDL_FreeSurface(map);
	SDL_FreeSurface(provinces);
}

int PlayerController::AdvanceDate(void* ref){
	PlayerController* reference = static_cast<PlayerController*>(ref);
	int x = 0;
	while (x < 10 && reference->Date.bIsPaused == false) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2 * 100 / reference->Date.Speed));
		x++;
	}

	//Advance by one day
	if (reference->Date.bIsPaused == false) {
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
				if (reference->Date.Year % 4 == 0.0) {
					reference->Date.MonthDays[1] = 29;
				} else {
					reference->Date.MonthDays[1] = 28;
				}
			}
		}
		reference->AdvanceDate(reference);
	}
	return 0;
}

void PlayerController::Pause(){
	/*Pause or unpause the game when this function is executed.
	It will always change the state of the Date.bIsPaused and
	set it to the opposite value.*/
	if (Date.bIsPaused == true) {
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
	if (change == true && Date.Speed < 4) {
		Date.Speed++;
	} else if (change == false && Date.Speed > 1) {
		Date.Speed--;
	}
}

void PlayerController::Tick(){
	//Execute the tick function for all countries (and all states)
	for (int x = 0; x < 58; x++) {
		CountriesArr[x]->Tick();
	}
}