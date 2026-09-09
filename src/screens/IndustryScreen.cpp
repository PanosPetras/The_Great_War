#include "screens/ScreenList.h"

#include "ui/Button.h"
#include "game/Country.h"
#include "ui/Label.h"
#include "core/MainWindow.h"

#include <array>
#include <cstddef>

namespace {
/*Backgrounds/Industry1.png is painted at this size, with a wooden plaque per
good and the right half of every plaque left blank for its number. Positions
below are in the art's own pixels and are scaled to the window, so a number
stays on its plaque whatever the window is.*/
constexpr int ArtWidth = 1920;
constexpr int ArtHeight = 1080;

struct StockpileLabel {
    // Which good this label displays
    Good good;

    // Where its number sits on the background art
    int x;
    int y;
};

/*Where each good is shown on the industry background. This is the single place
that knows the layout - both the initial labels and every refresh walk this
table, so the two can no longer drift apart. The rows below are the rows on the
screen: the raw materials, then what industry makes of them, then what the
population buys, then what the army takes. A good added here also needs its
plaque painted onto the background.*/
constexpr std::array<StockpileLabel, 38> Layout{{
    {Good::Coal, 341, 78}, {Good::Oil, 581, 78}, {Good::Timber, 821, 78}, {Good::Rubber, 1061, 78}, {Good::Cotton, 1301, 78}, {Good::Iron, 1541, 78},
    {Good::Copper, 461, 178}, {Good::Bauxite, 701, 178}, {Good::Nitrates, 941, 178}, {Good::Grain, 1181, 178}, {Good::Fruit, 1421, 178},
    {Good::Steel, 461, 328}, {Good::Aluminum, 701, 328}, {Good::Electric_gear, 941, 328}, {Good::Machine_parts, 1181, 328}, {Good::Engines, 1421, 328},
    {Good::Boilers, 461, 428}, {Good::Glass, 701, 428}, {Good::Lumber, 941, 428}, {Good::Cement, 1181, 428}, {Good::Canvas, 1421, 428},
    {Good::Canned_food, 461, 578}, {Good::Clothes, 701, 578}, {Good::Liquor, 941, 578}, {Good::Furniture, 1181, 578}, {Good::Paper, 1421, 578},
    {Good::Telephones, 461, 678}, {Good::Radios, 701, 678}, {Good::Automobiles, 941, 678}, {Good::Merchant_ships, 1181, 678}, {Good::Fuel, 1421, 678},
    {Good::Small_arms, 581, 828}, {Good::Ammunition, 821, 828}, {Good::Artillery, 1061, 828}, {Good::Explosives, 1301, 828},
    {Good::Tanks, 701, 928}, {Good::Planes, 941, 928}, {Good::Airship, 1181, 928},
}};
} // namespace

IndustryScreen::IndustryScreen(MainWindow& mw, Country* Pl) : Screen(mw), Player(Pl) {
    SetupBg("Backgrounds/Industry1.png");
    auto [Width, Height] = mw.GetWindowDimensions();

    for(const auto& [good, x, y] : Layout) {
        AddLabel<Label>(mw, std::to_string(Player->Stock[good]), FontSize::Heading, Width * x / ArtWidth, Height * y / ArtHeight);
    }
}

void IndustryScreen::Update(Uint32) {
    for(std::size_t i = 0; i < Layout.size(); ++i) {
        LabelArr[i]->ChangeText(std::to_string(Player->Stock[Layout[i].good]));
    }
}
