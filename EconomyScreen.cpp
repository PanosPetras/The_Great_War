#include "Button.h"
#include "Label.h"
#include "MainWindow.h"
#include "PlayerController.h"
#include "ScreenList.h"
#include "Slider.h"

EconomyScreen::EconomyScreen(MainWindow& mw, Country* Pl) : Screen(mw), Player(Pl) {
    SetupBg("Backgrounds/Industry.png");
    auto [Width, Height] = mw.GetWindowDimensions();

    AddLabel<Label>(*main_window, "Current Funds: " + std::to_string(Pl->Stock.Money), 32, int(0.1 * Width), int(0.1 * Height));
    AddLabel<Label>(*main_window, "Tax Rate: " + std::to_string(Pl->policy.TaxRate) + '%', 32, int(0.1 * Width), int(0.2 * Height));
    AddLabel<Label>(*main_window, "Healthcare: " + std::to_string(Pl->policy.Healthcare) + '%', 32, int(0.4 * Width), int(0.2 * Height));
    AddLabel<Label>(*main_window, "Education: " + std::to_string(Pl->policy.TaxRate) + '%', 32, int(0.4 * Width), int(0.4 * Height));
    AddLabel<Label>(*main_window, "Research: " + std::to_string(Pl->policy.TaxRate) + '%', 32, int(0.4 * Width), int(0.6 * Height));

    AddDrawable<Slider>(*main_window, int(0.11 * Width), int(0.25 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->policy.TaxRate, [this] { OnTaxRateChanged(); });
    AddDrawable<Slider>(*main_window, int(0.41 * Width), int(0.25 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->policy.Healthcare, [this] { OnHealthcareChanged(); });
}

void EconomyScreen::Update() {
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
