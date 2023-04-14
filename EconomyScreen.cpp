#include "PlayerController.h"
#include "ScreenList.h"
#include "MainWindow.h"
#include "Button.h"
#include "Slider.h"
#include "Label.h"

EconomyScreen::EconomyScreen(SDL_Renderer_ctx& r, Country* Pl) : Screen(r), Player(Pl) {
	SetupBg("Backgrounds/Industry.png");
        auto [Width, Height] = GetWindowDimensions();

	AddLabel<Label>(r, "Current Funds: " + std::to_string(Pl->Stock.Money), 32, int(0.1 * Width), int(0.1 * Height));
	AddLabel<Label>(r, "Tax Rate: " + std::to_string(Pl->policy.TaxRate) + '%', 32, int(0.1 * Width), int(0.2 * Height));
	AddLabel<Label>(r, "Healthcare: " + std::to_string(Pl->policy.Healthcare) + '%', 32, int(0.4 * Width), int(0.2 * Height));
	AddLabel<Label>(r, "Education: " + std::to_string(Pl->policy.TaxRate) + '%', 32, int(0.4 * Width), int(0.4 * Height));
	AddLabel<Label>(r, "Research: " + std::to_string(Pl->policy.TaxRate) + '%', 32, int(0.4 * Width), int(0.6 * Height));

	AddDrawable<Slider>(r, int(0.11 * Width), int(0.25 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->policy.TaxRate, [this]{OnTaxRateChanged();});
	AddDrawable<Slider>(r, int(0.41 * Width), int(0.25 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->policy.Healthcare, [this]{OnHealthcareChanged();});
}

void EconomyScreen::Update(){
	std::string l1 = "Current Funds: " + std::to_string(Player->Stock.Money);
	LabelArr[0]->ChangeText(l1.c_str());
}

void EconomyScreen::OnTaxRateChanged() {
	Slider& slider = As<Slider>(0);
	std::string l1 = "Tax Rate: " + std::to_string(slider.Values.Value) + "%";
	LabelArr[1]->ChangeText(l1.c_str());
	Player->policy.TaxRate = slider.Values.Value;
}

void EconomyScreen::OnHealthcareChanged() {
	Slider& slider = As<Slider>(1);
	std::string l1 = "Healthcare: " + std::to_string(slider.Values.Value) + "%";
	LabelArr[2]->ChangeText(l1.c_str());

	Player->policy.Healthcare = slider.Values.Value;
}
