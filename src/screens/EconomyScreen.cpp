#include "ui/Button.h"
#include "ui/Label.h"
#include "core/MainWindow.h"
#include "game/PlayerController.h"
#include "screens/ScreenList.h"
#include "ui/Slider.h"

EconomyScreen::EconomyScreen(MainWindow& mw, Country* Pl) : Screen(mw), Player(Pl) {
    SetupBg("Backgrounds/Industry.png");
    auto [Width, Height] = mw.GetWindowDimensions();

    AddLabel<Label>(*main_window, "Current Funds: " + std::to_string(Pl->Money), FontSize::Heading, int(0.1 * Width), int(0.1 * Height));
    AddLabel<Label>(*main_window, "Tax Rate: " + std::to_string(Pl->policy.TaxRate) + '%', FontSize::Heading, int(0.1 * Width), int(0.2 * Height));
    AddLabel<Label>(*main_window, "Healthcare: " + std::to_string(Pl->policy.Healthcare) + '%', FontSize::Heading, int(0.4 * Width), int(0.2 * Height));
    AddLabel<Label>(*main_window, "Education: " + std::to_string(Pl->policy.TaxRate) + '%', FontSize::Heading, int(0.4 * Width), int(0.4 * Height));
    AddLabel<Label>(*main_window, "Research: " + std::to_string(Pl->policy.TaxRate) + '%', FontSize::Heading, int(0.4 * Width), int(0.6 * Height));
    AddLabel<Label>(*main_window, StandardOfLivingText(), FontSize::Heading, int(0.1 * Width), int(0.4 * Height));
    AddLabel<Label>(*main_window, BudgetText(), FontSize::Caption, int(0.1 * Width), int(0.8 * Height));

    AddDrawable<Slider>(*main_window, int(0.11 * Width), int(0.25 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->policy.TaxRate, [this] { OnTaxRateChanged(); });
    AddDrawable<Slider>(*main_window, int(0.41 * Width), int(0.25 * Height), int(0.1 * Width), int(0.035 * Height), 0, 100, Pl->policy.Healthcare, [this] { OnHealthcareChanged(); });
}

void EconomyScreen::Update(Uint32) {
    std::string l1 = "Current Funds: " + std::to_string(Player->Money);
    LabelArr[0]->ChangeText(l1.c_str());
    LabelArr[5]->ChangeText(StandardOfLivingText());
    LabelArr[6]->ChangeText(BudgetText());
}

std::string EconomyScreen::BudgetText() const {
    const Budget& budget = Player->GetBudget();
    const long long net = budget.Net();
    return "Yesterday: " + std::string(net >= 0 ? "+" : "") + std::to_string(net) + "  (taxes +" + std::to_string(budget.taxes) + ", healthcare -" + std::to_string(budget.healthcare) + ", factories -" +
           std::to_string(budget.factories) + ')';
}

std::string EconomyScreen::StandardOfLivingText() const {
    return "Standard of living: " + std::to_string(Player->GetSatisfaction() / 10) + '%';
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
