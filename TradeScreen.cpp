#include "ScreenList.h"

TradeScreen::TradeScreen(SDL_Renderer* r, Country* Pl) : Screen(r) {
	SetupBg("Backgrounds/Industry.png");
	Player = Pl;
}

void TradeScreen::Update() {

}
