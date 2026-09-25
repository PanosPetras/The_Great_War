#include "screens/ScreenList.h"

#include "ui/Button.h"
#include "game/Country.h"
#include "ui/Label.h"
#include "core/MainWindow.h"
#include "screens/GoodsLayout.h"

#include <array>
#include <cstddef>

IndustryScreen::IndustryScreen(MainWindow& mw, Country* Pl) : Screen(mw), Player(Pl) {
    SetupBg("Backgrounds/Industry1.png");
    auto [Width, Height] = mw.GetWindowDimensions();

    for(const auto& [good, x, y] : GoodsLayout::Layout) {
        AddLabel<Label>(mw, std::to_string(Player->Stock[good]), FontSize::Heading, Width * x / GoodsLayout::ArtWidth, Height * y / GoodsLayout::ArtHeight);
    }
}

void IndustryScreen::Update(Uint32) {
    for(std::size_t i = 0; i < GoodsLayout::Layout.size(); ++i) {
        LabelArr[i]->ChangeText(std::to_string(Player->Stock[GoodsLayout::Layout[i].good]));
    }
}
