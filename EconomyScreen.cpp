#include "ScreenList.h"
#include "WindowInfo.h"

EconomyScreen::EconomyScreen(SDL_Renderer* r, Country* Pl) : Screen(r) {
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");
	Player = Pl;

	int Width = GetWindowWidth(), Height = GetWindowHeight();

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

	auto change = std::bind(&EconomyScreen::OnTaxRateChanged, this);

	InputDrawableArr[0] = new Slider(r, int(0.11 * Width), int(0.25 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->policy.TaxRate, change);

	change = std::bind(&EconomyScreen::OnHealthcareChanged, this);
	InputDrawableArr[1] = new Slider(r, int(0.41 * Width), int(0.25 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->policy.Healthcare, change);

	LabelArrtop = 5;
	ImageArrtop = 0;
	InputDrawableArrtop = 2;
}

void EconomyScreen::Update(){
	std::string l1 = "Current Funds: " + std::to_string(Player->Stock.Money);
	LabelArr[0]->ChangeText(l1.c_str());
}

void EconomyScreen::OnTaxRateChanged() {
	Slider* slider = (Slider*)InputDrawableArr[0];
	std::string l1 = "Tax Rate: " + std::to_string(slider->Values.Value) + "%";
	LabelArr[1]->ChangeText(l1.c_str());
	Player->policy.TaxRate = slider->Values.Value;
}

void EconomyScreen::OnHealthcareChanged() {
	Slider* slider = (Slider*)InputDrawableArr[1];
	std::string l1 = "Healthcare: " + std::to_string(slider->Values.Value) + "%";
	LabelArr[2]->ChangeText(l1.c_str());

	Player->policy.Healthcare = slider->Values.Value;
}