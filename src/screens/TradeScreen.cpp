#include "screens/ScreenList.h"

#include "ui/Button.h"

TradeScreen::TradeScreen(MainWindow& mw, Country* Pl) : Screen(mw) {
    SetupBg("Backgrounds/Industry.png");
    Player = Pl;
}

void TradeScreen::Update(Uint32) {}
