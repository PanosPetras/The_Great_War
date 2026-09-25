#include "screens/ScreenList.h"

#include "core/MainWindow.h"
#include "game/Market.h"
#include "screens/GoodsLayout.h"
#include "ui/Label.h"

#include <cstddef>
#include <cstdio>
#include <string>

namespace {
/*A price to the penny while it is small enough for pennies to matter, and to
the unit once they do not - a merchant ship costs thousands.*/
std::string PriceText(double price) {
    char text[32];
    std::snprintf(text, sizeof text, price < 100 ? "%.2f" : "%.0f", price);
    return text;
}
} // namespace

TradeScreen::TradeScreen(MainWindow& mw, const Market& market) : Screen(mw), WorldMarket(market) {
    SetupBg("Backgrounds/Industry1.png");
    auto [Width, Height] = mw.GetWindowDimensions();

    // Today's price on every good's plaque, where the Industry screen puts what is held
    for(const auto& [good, x, y] : GoodsLayout::Layout) {
        AddLabel<Label>(mw, PriceText(WorldMarket.Price[good]), FontSize::Heading, Width * x / GoodsLayout::ArtWidth, Height * y / GoodsLayout::ArtHeight);
    }
}

void TradeScreen::Update(Uint32) {
    for(std::size_t i = 0; i < GoodsLayout::Layout.size(); ++i) {
        LabelArr[i]->ChangeText(PriceText(WorldMarket.Price[GoodsLayout::Layout[i].good]));
    }
}
