#include "ScreenList.h"

EconomyScreen::EconomyScreen(SDL_Renderer* r, int Width, int Height, Country* Pl) : Screen(r, Width, Height) {
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");
	Player = Pl;

	std::string l1 = "Current Funds: " + std::to_string(Pl->Stock.Money);
	LabelArr[0] = new Label(r, l1.c_str(), 32, int(0.1 * Width), int(0.1 * Height));
	l1 = "Tax Rate: " + std::to_string(Pl->Policy.TaxRate) + "%";
	LabelArr[1] = new Label(r, l1.c_str(), 32, int(0.1 * Width), int(0.2 * Height));
	l1 = "Healthcare: "+ std::to_string(Pl->Policy.Healthcare) + "%";
	LabelArr[2] = new Label(r, l1.c_str(), 32, int(0.4 * Width), int(0.2 * Height));
	l1 = "Eduaction: ", std::to_string(Pl->Policy.TaxRate), "%";
	LabelArr[3] = new Label(r, l1.c_str(), 32, int(0.4 * Width), int(0.4 * Height));
	l1 = "Research: ", std::to_string(Pl->Policy.TaxRate), "%";
	LabelArr[4] = new Label(r, l1.c_str(), 32, int(0.4 * Width), int(0.6 * Height));
	LabelArrtop = 5;
	ImageArrtop = 0;
	ButtonArrtop = 0;
	SliderArr[0] = new Slider(r, int(0.12 * Width), int(0.3 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->Policy.TaxRate);
	SliderArr[1] = new Slider(r, int(0.42 * Width), int(0.3 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->Policy.Healthcare);
	SliderArrtop = 2;
}

void EconomyScreen::Update(){
	std::string l1 = "Current Funds: " + std::to_string(Player->Stock.Money);
	LabelArr[0]->ChangeText(l1.c_str());

	l1 = "Tax Percentage: " + std::to_string(SliderArr[0]->Values.Value) + "%";
	LabelArr[1]->ChangeText(l1.c_str());

	l1 = "Healthcare: " + std::to_string(SliderArr[1]->Values.Value) + "%";
	LabelArr[2]->ChangeText(l1.c_str());

	Player->Policy.TaxRate = SliderArr[0]->Values.Value;
	Player->Policy.Healthcare = SliderArr[1]->Values.Value;
}