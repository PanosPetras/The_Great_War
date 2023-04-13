#include "ScreenList.h"

TradeScreen::TradeScreen(SDL_Renderer_ctx& r, Country* Pl) : Screen(r) {
	SetupBg("Backgrounds/Industry.png");
	Player = Pl;
}

void TradeScreen::Update() {

}
