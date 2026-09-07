#include "ScreenList.h"

#include "Button.h"
#include "Country.h"
#include "Label.h"
#include "MainWindow.h"

#include <array>
#include <cstddef>

namespace {
struct StockpileLabel {
    // Which good this label displays
    Good good;

    // Position, as a fraction of the window's dimensions
    double x;
    double y;
};

/*Where each good is shown on the industry background. This is the single place
that knows the layout - both the initial labels and every refresh walk this
table, so the two can no longer drift apart.*/
constexpr std::array<StockpileLabel, 30> Layout{{
    {Good::Coal, 0.24, 0.165},         {Good::Iron, 0.365, 0.165},         {Good::Timber, 0.49, 0.165},      {Good::Cotton, 0.615, 0.165},     {Good::Rubber, 0.24, 0.26},         {Good::Oil, 0.365, 0.26},
    {Good::Lumber, 0.49, 0.26},        {Good::Glass, 0.615, 0.26},         {Good::Grain, 0.74, 0.21},        {Good::Canned_food, 0.24, 0.395}, {Good::Clothes, 0.365, 0.395},      {Good::Liquor, 0.49, 0.395},
    {Good::Furniture, 0.615, 0.395},   {Good::Paper, 0.24, 0.49},          {Good::Telephones, 0.365, 0.49},  {Good::Fruit, 0.49, 0.49},        {Good::Radios, 0.615, 0.49},        {Good::Automobiles, 0.74, 0.44},
    {Good::Machine_parts, 0.24, 0.63}, {Good::Electric_gear, 0.365, 0.63}, {Good::Fuel, 0.49, 0.63},         {Good::Cement, 0.615, 0.63},      {Good::Merchant_ships, 0.74, 0.63}, {Good::Small_arms, 0.24, 0.765},
    {Good::Ammunition, 0.365, 0.765},  {Good::Artillery, 0.49, 0.765},     {Good::Explosives, 0.615, 0.765}, {Good::Tanks, 0.74, 0.765},       {Good::Planes, 0.49, 0.86},         {Good::Airship, 0.365, 0.86},
}};
} // namespace

IndustryScreen::IndustryScreen(MainWindow& mw, Country* Pl) : Screen(mw), Player(Pl) {
    SetupBg("Backgrounds/Industry1.png");
    auto [Width, Height] = mw.GetWindowDimensions();

    for(const auto& [good, x, y] : Layout) {
        AddLabel<Label>(mw, std::to_string(Player->Stock[good]), FontSize::Heading, int(Width * x), int(Height * y));
    }
}

void IndustryScreen::Update(Uint32) {
    for(std::size_t i = 0; i < Layout.size(); ++i) {
        LabelArr[i]->ChangeText(std::to_string(Player->Stock[Layout[i].good]));
    }
}
