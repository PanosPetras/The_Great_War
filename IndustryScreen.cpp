#include "ScreenList.h"

#include "Button.h"
#include "Country.h"
#include "Label.h"
#include "MainWindow.h"

#include <array>
#include <cstddef>

namespace {
struct StockpileLabel {
    // Which entry of the snapshot below this label displays
    std::size_t resource;

    // Position, as a fraction of the window's dimensions
    double x;
    double y;
};

/*Where each resource is shown on the industry background. This is the single
place that knows the layout - both the initial labels and every refresh walk
this table, so the two can no longer drift apart.*/
constexpr std::array<StockpileLabel, 30> Layout{{
    {0, 0.24, 0.165},   {5, 0.365, 0.165}, {2, 0.49, 0.165},   {4, 0.615, 0.165}, {3, 0.24, 0.26},    {1, 0.365, 0.26},  {11, 0.49, 0.26},   {10, 0.615, 0.26}, {6, 0.74, 0.21},  {19, 0.24, 0.395},
    {21, 0.365, 0.395}, {28, 0.49, 0.395}, {20, 0.615, 0.395}, {27, 0.24, 0.49},  {25, 0.365, 0.49},  {7, 0.49, 0.49},   {24, 0.615, 0.49},  {22, 0.74, 0.44},  {9, 0.24, 0.63},  {8, 0.365, 0.63},
    {26, 0.49, 0.63},   {12, 0.615, 0.63}, {23, 0.74, 0.63},   {16, 0.24, 0.765}, {13, 0.365, 0.765}, {17, 0.49, 0.765}, {15, 0.615, 0.765}, {18, 0.74, 0.765}, {14, 0.49, 0.86}, {29, 0.365, 0.86},
}};

/*The country's stockpile flattened into the order the Layout table indexes by.
This is the last place the 30 fields are spelled out by hand; it goes away once
Stockpile itself becomes indexable.*/
std::array<int, Layout.size()> Snapshot(const Country& country) {
    const Stockpile& s = country.Stock;

    return {s.Coal,       s.Oil,        s.Timber,    s.Rubber, s.Cotton,      s.Iron,      s.Grain,   s.Fruit,       s.Electric_gear,  s.Machine_parts, s.Glass,      s.Lumber, s.Cement, s.Ammunition, s.Planes,
            s.Explosives, s.Small_arms, s.Artillery, s.Tanks,  s.Canned_food, s.Furniture, s.Clothes, s.Automobiles, s.Merchant_ships, s.Radios,        s.Telephones, s.Fuel,   s.Paper,  s.Liquor,     s.Airship};
}
} // namespace

IndustryScreen::IndustryScreen(MainWindow& mw, Country* Pl) : Screen(mw), Player(Pl) {
    SetupBg("Backgrounds/Industry1.png");
    auto [Width, Height] = mw.GetWindowDimensions();

    const auto stock = Snapshot(*Player);

    for(const auto& [resource, x, y] : Layout) {
        AddLabel<Label>(mw, std::to_string(stock[resource]), 32, int(Width * x), int(Height * y));
    }
}

void IndustryScreen::Update(Uint32) {
    const auto stock = Snapshot(*Player);

    for(std::size_t i = 0; i < Layout.size(); ++i) {
        LabelArr[i]->ChangeText(std::to_string(stock[Layout[i].resource]));
    }
}
