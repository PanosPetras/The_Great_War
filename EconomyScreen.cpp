#include "ScreenList.h"

EconomyScreen::EconomyScreen(SDL_Renderer* r, int Width, int Height, Country* Pl) : Screen(r, Width, Height) {
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");
	Player = Pl;

	std::string l1 = "Current Funds: " + std::to_string(Pl->Stock.Money);
	LabelArr[0] = new Label(r, l1.c_str(), 32, int(0.1 * Width), int(0.1 * Height));
	l1 = "Tax Rate: " + std::to_string(Pl->policy.TaxRate) + "%";
	LabelArr[1] = new Label(r, l1.c_str(), 32, int(0.1 * Width), int(0.2 * Height));
	l1 = "Healthcare: "+ std::to_string(Pl->policy.Healthcare) + "%";
	LabelArr[2] = new Label(r, l1.c_str(), 32, int(0.4 * Width), int(0.2 * Height));
	l1 = "Education: ", std::to_string(Pl->policy.TaxRate), "%";
	LabelArr[3] = new Label(r, l1.c_str(), 32, int(0.4 * Width), int(0.4 * Height));
	l1 = "Research: ", std::to_string(Pl->policy.TaxRate), "%";
	LabelArr[4] = new Label(r, l1.c_str(), 32, int(0.4 * Width), int(0.6 * Height));
	LabelArrtop = 5;
	ImageArrtop = 0;
	ButtonArrtop = 0;
	auto change = std::bind(&EconomyScreen::OnTaxRateChanged, this);
	SliderArr[0] = new Slider(r, int(0.11 * Width), int(0.25 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->policy.TaxRate, change);
	change = std::bind(&EconomyScreen::OnHealthcareChanged, this);
	SliderArr[1] = new Slider(r, int(0.41 * Width), int(0.25 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->policy.Healthcare, change);
	SliderArrtop = 2;
}

void EconomyScreen::Update(){
	std::string l1 = "Current Funds: " + std::to_string(Player->Stock.Money);
	LabelArr[0]->ChangeText(l1.c_str());
}

void EconomyScreen::OnTaxRateChanged() {
	std::string l1 = "Tax Rate: " + std::to_string(SliderArr[0]->Values.Value) + "%";
	LabelArr[1]->ChangeText(l1.c_str());
	Player->policy.TaxRate = SliderArr[0]->Values.Value;
}

void EconomyScreen::OnHealthcareChanged() {
	std::string l1 = "Healthcare: " + std::to_string(SliderArr[1]->Values.Value) + "%";
	LabelArr[2]->ChangeText(l1.c_str());

	Player->policy.Healthcare = SliderArr[1]->Values.Value;
}