#include "UI.h"

#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "PlayerController.h"
#include "Screen.h"
#include "ScreenList.h"
#include "ToggleButton.h"

UI::UI(SDL_Renderer_ctx& r, const char* tag, PlayerController* PC, std::function<void(std::unique_ptr<Screen>, std::string)> fpl) : renderer(r) {
        auto [Width, Height] = GetWindowDimensions();

	//The country management tabs
	std::string flg = std::string("Flags/") + tag;
	flag = std::make_unique<Button>(r, int(Width * 0.005), int(Height * 0.005), int(Width * 0.05), int(Height * 0.05), flg.c_str(), nullptr, SDLK_q);

	Buttons[0] = std::make_unique<Button>(r, int(Width * 0.1), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Technology", nullptr, SDLK_w);
	Buttons[1] = std::make_unique<Button>(r, int(Width * 0.15), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Economy", [this]{OpenEconomyScreen();}, SDLK_e);
	Buttons[3] = std::make_unique<Button>(r, int(Width * 0.2), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Industry", [this]{OpenIndustryScreen();}, SDLK_r);
	Buttons[2] = std::make_unique<Button>(r, int(Width * 0.25), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Trade", [this]{OpenTradeScreen();}, SDLK_t);
	Buttons[4] = std::make_unique<Button>(r, int(Width * 0.3), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Diplomacy", [this]{OpenDiplomacyScreen();}, SDLK_y);
	Buttons[5] = std::make_unique<Button>(r, int(Width * 0.35), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Military", nullptr, SDLK_u);

	flagbg = std::make_unique<Image>(r, "Backgrounds/FlagBg.png", 0, 0, int(Width * 0.06), int(Height * 0.06));

	//The date tab
	Date = std::make_unique<Label>(r, "1/1/1910", 32, int(Width * 0.81), 0);
	DateButtons[0] = std::make_unique<Button>(r, int(Width * 0.7), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/Subtract", [this]{DecreaseSpeed();}, SDLK_KP_MINUS);

	DateButtons[1] = std::make_unique<Button>(r, int(Width * 0.9), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/Increment", [this]{IncreaseSpeed();}, SDLK_KP_PLUS);

	PauseButton = std::make_unique<ToggleButton>(r, int(Width * 0.77), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/pause", "Buttons/UI/play", [this](bool p){ PauseDate(p); }, SDLK_SPACE);

	SpeedBg = std::make_unique<Image>(r, "Backgrounds/FlagBg.png", int(Width * 0.7), 0, int(Width * 0.222), int(Height * 0.04));
	SpeedImg = std::make_unique<Image>(r, "Icons/1-Speed.png", int(Width * 0.735), 0, int(Width * 0.022), int(Height * 0.04));

	//A reference to the player controller
	PCref = PC;

	ChangeScreenFunc = fpl;
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

void UI::Handle_Input(SDL_Event& ev){
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

void UI::PauseDate(bool){
	PCref->Pause();
}

void UI::OpenDiplomacyScreen() {
	ChangeScreenFunc(std::make_unique<DiplomacyScreen>(renderer, PCref), "DiplomacyScreen");
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
	ChangeScreenFunc(std::make_unique<IndustryScreen>(renderer, Res), "IndustryScreen");
}

void UI::OpenEconomyScreen() {
	ChangeScreenFunc(std::make_unique<EconomyScreen>(renderer, PCref->CountriesArr.at(PCref->player_index).get()), "EconomyScreen");
}

void UI::OpenTradeScreen() {
	ChangeScreenFunc(std::make_unique<TradeScreen>(renderer, PCref->CountriesArr.at(PCref->player_index).get()), "TradeScreen");
}
