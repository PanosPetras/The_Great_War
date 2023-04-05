#include "ScreenList.h"

TradeScreen::TradeScreen(SDL_Renderer* r, Country* Pl) : Screen(r) {
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");
	Player = Pl;

	LabelArrtop = 0;
	ImageArrtop = 0;
	InputDrawableArrtop = 0;
}

void TradeScreen::Update() {

}