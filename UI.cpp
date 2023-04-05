#include "ScreenList.h"
#include "Screen.h"
#include "PlayerController.h"
#include "ToggleButton.h"
#include "WindowInfo.h"

UI::UI(SDL_Renderer* r, const char* tag, PlayerController* PC, std::function<void(Screen*, std::string)> fpl) {
	renderer = r;

	std::string str = tag;
	int Width = GetWindowWidth(), Height = GetWindowHeight();

	//The country management tabs
	std::string flg = "Flags/" + str;
	flag = new Button(r, int(Width * 0.005), int(Height * 0.005), int(Width * 0.05), int(Height * 0.05), flg.c_str(), NULL, SDLK_q);

	Buttons[0] = new Button(r, int(Width * 0.1), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Technology", NULL, SDLK_w);

	auto change = std::bind(&UI::OpenEconomyScreen, this);
	Buttons[1] = new Button(r, int(Width * 0.15), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Economy", change, SDLK_e);

	change = std::bind(&UI::OpenIndustryScreen, this);
	Buttons[3] = new Button(r, int(Width * 0.2), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Industry", change, SDLK_r);

	change = std::bind(&UI::OpenTradeScreen, this);
	Buttons[2] = new Button(r, int(Width * 0.25), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Trade", change, SDLK_t);

	change = std::bind(&UI::OpenDiplomacyScreen, this);
	Buttons[4] = new Button(r, int(Width * 0.3), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Diplomacy", change, SDLK_y);

	Buttons[5] = new Button(r, int(Width * 0.35), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Military", NULL, SDLK_u);

	flagbg = new Image(r, "Backgrounds/FlagBg.png", 0, 0, int(Width * 0.06), int(Height * 0.06));

	//The date tab
	Date = new Label(r, "1/1/1910", 32, int(Width * 0.81), 0);
	change = std::bind(&UI::DecreaseSpeed, this);
	DateButtons[0] = new Button(r, int(Width * 0.7), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/Subtract", change, SDLK_KP_MINUS);

	change = std::bind(&UI::IncreaseSpeed, this);
	DateButtons[1] = new Button(r, int(Width * 0.9), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/Increment", change, SDLK_KP_PLUS);

	auto change1 = std::bind(&UI::PauseDate, this, std::placeholders::_1);
	PauseButton = new ToggleButton(r, int(Width * 0.77), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/pause", "Buttons/UI/play", change1, SDLK_SPACE);

	SpeedBg = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.7), 0, int(Width * 0.222), int(Height * 0.04));
	SpeedImg = new Image(r, "Icons/1-Speed.png", int(Width * 0.735), 0, int(Width * 0.022), int(Height * 0.04));

	//A reference to the player controller
	PCref = PC;

	ChangeScreenFunc = fpl;
}

UI::~UI(){
	//Delete all objects from memory before we destroy this object
	delete flag, flagbg;
	for (auto btn : Buttons) {
		delete btn;
	}

	for (auto btn : DateButtons) {
		delete btn;
	}
	delete PauseButton, Date, SpeedBg, SpeedImg;
}

void UI::Render(){
	//Renders the country menu
	flagbg->Draw();
	flag->Draw();
	for (int x = 0; x < 5; x++) {
		Buttons[x]->Draw();
	}

	//Renders the date menu
	SpeedBg->Draw();
	std::string str = std::to_string(PCref->Date.Day) + "-" + std::to_string(PCref->Date.Month) + "-" + std::to_string(PCref->Date.Year);
	if (Date->GetText() != str) {
		Date->ChangeText(str.c_str());
	}
	Date->Draw();
	SpeedImg->Draw();
	for (int x = 0; x < 2; x++) {
		DateButtons[x]->Draw();
	}
	PauseButton->Draw();
}

void UI::Handle_Input(SDL_Event* ev){
	//Handles inputs for buttons
	flag->HandleInput(ev);

	for (int x = 0; x < 5; x++) {
		Buttons[x]->HandleInput(ev);
	}

	for (int x = 0; x < 2; x++) {
		DateButtons[x]->HandleInput(ev);
	}

	PauseButton->HandleInput(ev);
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

void UI::PauseDate(bool _){
	PCref->Pause();
}

void UI::OpenDiplomacyScreen() {
	Screen* NS = new DiplomacyScreen(renderer, PCref);
	ChangeScreenFunc(NS, "DiplomacyScreen");
}

void UI::OpenIndustryScreen(){
	int Res[30] = {	PCref->CountriesArr.at(PCref->player_index)->Stock.Coal,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Oil,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Timber,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Rubber,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Cotton,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Iron,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Grain,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Fruit,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Electric_gear,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Machine_parts,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Glass,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Lumber,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Cement,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Ammunition,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Planes,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Explosives,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Small_arms,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Artillery,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Tanks,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Canned_food,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Furniture,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Clothes,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Automobiles,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Merchant_ships,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Radios,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Telephones,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Fuel,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Paper,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Liquor,
					PCref->CountriesArr.at(PCref->player_index)->Stock.Airship};
	Screen* NS = new IndustryScreen(renderer, Res);
	ChangeScreenFunc(NS, "IndustryScreen");
}

void UI::OpenEconomyScreen() {
	Screen* NS = new EconomyScreen(renderer, PCref->CountriesArr.at(PCref->player_index));
	ChangeScreenFunc(NS, "EconomyScreen");
}

void UI::OpenTradeScreen() {
	Screen* NS = new TradeScreen(renderer, PCref->CountriesArr.at(PCref->player_index));
	ChangeScreenFunc(NS, "TradeScreen");
}