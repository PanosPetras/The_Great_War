#include "ScreenList.h"

UI::UI(SDL_Renderer* r, int Width, int Height, const char* tag, PlayerController* PC, std::function<void(Screen*)> fpl){
	renderer = r;
	WindowSize[0] = Width;
	WindowSize[1] = Height;

	std::string str = tag;

	//The country management tabs
	std::string flg = "Buttons/Flags/" + str;
	flag = new Button(r, int(Width * 0.005), int(Height * 0.005), int(Width * 0.05), int(Height * 0.05), flg.c_str(), NULL, SDLK_q);
	Buttons[0] = new Button(r, int(Width * 0.1), 0, int(Width * 0.035), int(Height * 0.05), "Buttons/UI/Technology", NULL, SDLK_w);
	auto change = std::bind(&UI::OpenEconomyScreen, this);
	Buttons[1] = new Button(r, int(Width * 0.15), 0, int(Width * 0.035), int(Height * 0.05), "Buttons/UI/Economy", change, SDLK_e);
	change = std::bind(&UI::OpenIndustryScreen, this);
	Buttons[2] = new Button(r, int(Width * 0.2), 0, int(Width * 0.035), int(Height * 0.05), "Buttons/UI/Industry", change, SDLK_r);
	Buttons[3] = new Button(r, int(Width * 0.25), 0, int(Width * 0.035), int(Height * 0.05), "Buttons/UI/Diplomacy", NULL, SDLK_t);
	Buttons[4] = new Button(r, int(Width * 0.3), 0, int(Width * 0.035), int(Height * 0.05), "Buttons/UI/Military", NULL, SDLK_y);

	flagbg = new Image(r, "Backgrounds/FlagBg.png", 0, 0, int(Width * 0.06), int(Height * 0.06));

	//The date tab
	Date = new Label(r, "1/1/1910", 32, int(Width * 0.81), 0);
	change = std::bind(&UI::DecreaseSpeed, this);
	DateButtons[0] = new Button(r, int(Width * 0.7), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/Subtract", change, SDLK_KP_MINUS);
	change = std::bind(&UI::IncreaseSpeed, this);
	DateButtons[1] = new Button(r, int(Width * 0.9), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/Increment", change, SDLK_KP_PLUS);
	change = std::bind(&UI::PauseDate, this);
	DateButtons[2] = new Button(r, int(Width * 0.77), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/play", change, SDLK_SPACE);
	SpeedBg = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.7), 0, int(Width * 0.222), int(Height * 0.04));
	SpeedImg = new Image(r, "Icons/1-Speed.png", int(Width * 0.735), 0, int(Width * 0.022), int(Height * 0.04));

	bDateUpdated = false;

	//A reference to the player controller
	PCref = PC;

	ChangeScreenFunc = fpl;
}

UI::~UI(){
	//Delete all objects from memory before we destroy this object
	delete flag, flagbg;
	for (int x = 0; x < 5; x++) {
		delete Buttons[x];
	}

	for (int x = 0; x < 3; x++) {
		delete DateButtons[x];
	}
	delete Date, SpeedBg, SpeedImg;
}

void UI::Render(){
	//Renders the country menu
	flagbg->RenderImage();
	flag->RenderButton();
	for (int x = 0; x < 5; x++) {
		Buttons[x]->RenderButton();
	}

	//Renders the date menu
	SpeedBg->RenderImage();
	std::string str = std::to_string(PCref->Date.Day) + "-" + std::to_string(PCref->Date.Month) + "-" + std::to_string(PCref->Date.Year);
	if (Date->text != str) {
		bDateUpdated = true;
	} else {
		bDateUpdated = false;
	}
	Date->ChangeText(str.c_str());
	Date->RenderLabel();
	SpeedImg->RenderImage();
	for (int x = 0; x < 3; x++) {
		DateButtons[x]->RenderButton();
	}
}

void UI::Handle_Input(SDL_Event* ev){
	//Handles inputs for buttons
	flag->button_process_event(ev);
	for (int x = 0; x < 5; x++) {
		Buttons[x]->button_process_event(ev);
	}
	for (int x = 0; x < 3; x++) {
		DateButtons[x]->button_process_event(ev);
	}
}

void UI::IncreaseSpeed(){
	PCref->ChangeSpeed(true);
	std::string str = "Icons/" + std::to_string(PCref->Date.Speed) + "-Speed.png";
	SpeedImg->ChangeImage(str.c_str());
}

void UI::DecreaseSpeed(){
	PCref->ChangeSpeed(false);
	std::string str = "Icons/" + std::to_string(PCref->Date.Speed) + "-Speed.png";
	SpeedImg->ChangeImage(str.c_str());
}

void UI::PauseDate(){
	PCref->Pause();
	if (PCref->Date.bIsPaused == true) {
		DateButtons[2]->ChangeImage("Buttons/UI/play");
	} else {
		DateButtons[2]->ChangeImage("Buttons/UI/pause");
	}
}

void UI::OpenIndustryScreen(){
	/*int Res[30] = {	PCref->CountriesArr[PCref->player_index]->Stock.Coal,
					PCref->CountriesArr[PCref->player_index]->Stock.Oil,
					PCref->CountriesArr[PCref->player_index]->Stock.Timber,
					PCref->CountriesArr[PCref->player_index]->Stock.Rubber,
					PCref->CountriesArr[PCref->player_index]->Stock.Cotton,
					PCref->CountriesArr[PCref->player_index]->Stock.Iron,
					PCref->CountriesArr[PCref->player_index]->Stock.Grain,
					PCref->CountriesArr[PCref->player_index]->Stock.Fruit,
					PCref->CountriesArr[PCref->player_index]->Stock.Electric_gear,
					PCref->CountriesArr[PCref->player_index]->Stock.Machine_parts,
					PCref->CountriesArr[PCref->player_index]->Stock.Glass,
					PCref->CountriesArr[PCref->player_index]->Stock.Lumber,
					PCref->CountriesArr[PCref->player_index]->Stock.Cement,
					PCref->CountriesArr[PCref->player_index]->Stock.Ammunition,
					PCref->CountriesArr[PCref->player_index]->Stock.Planes,
					PCref->CountriesArr[PCref->player_index]->Stock.Explosives,
					PCref->CountriesArr[PCref->player_index]->Stock.Small_arms,
					PCref->CountriesArr[PCref->player_index]->Stock.Artillery,
					PCref->CountriesArr[PCref->player_index]->Stock.Tanks,
					PCref->CountriesArr[PCref->player_index]->Stock.Canned_food,
					PCref->CountriesArr[PCref->player_index]->Stock.Furniture,
					PCref->CountriesArr[PCref->player_index]->Stock.Clothes,
					PCref->CountriesArr[PCref->player_index]->Stock.Automobiles,
					PCref->CountriesArr[PCref->player_index]->Stock.Merchant_ships,
					PCref->CountriesArr[PCref->player_index]->Stock.Radios,
					PCref->CountriesArr[PCref->player_index]->Stock.Telephones,
					PCref->CountriesArr[PCref->player_index]->Stock.Fuel,
					PCref->CountriesArr[PCref->player_index]->Stock.Paper,
					PCref->CountriesArr[PCref->player_index]->Stock.Liquor,
					PCref->CountriesArr[PCref->player_index]->Stock.Airship};
	Screen* NS = new IndustryScreen(renderer, WindowSize[0], WindowSize[1], Res, NULL, NULL);*/
	std::vector<std::string> str;
	std::vector<std::vector<std::string>> str1;
	std::vector<std::string> pp;
	for (int x = 0; x < 4; x++) {
		pp.push_back("");
	}

	//Save all the owned states' names and factories
	for (int x = 0; x < PCref->CountriesArr[PCref->player_index]->Country_State_Count; x++) {
		str.push_back(PCref->CountriesArr[PCref->player_index]->OwnedStates[x]->State_Name);
		for (int y = 0; y < 4; y++) {
			if (PCref->CountriesArr[PCref->player_index]->OwnedStates[x]->State_Factories[y] != nullptr) {
				pp[y] = PCref->CountriesArr[PCref->player_index]->OwnedStates[x]->State_Factories[y]->Type;
			} else {
				pp[y] = "";
			}
		}
		str1.push_back(pp);
	}

	Screen* NS = new FactoriesScreen(renderer, WindowSize[0], WindowSize[1], str, str1, NULL, NULL);
	ChangeScreenFunc(NS);
}

void UI::OpenEconomyScreen() {
	Screen* NS = new EconomyScreen(renderer, WindowSize[0], WindowSize[1], NULL, NULL);
	ChangeScreenFunc(NS);
}