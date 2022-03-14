#include "ScreenList.h"

TradeScreen::TradeScreen(SDL_Renderer* r, int Width, int Height, Country* Pl) : Screen(r, Width, Height) {
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");
	Player = Pl;

	LabelArrtop = 0;
	ImageArrtop = 0;
	ButtonArrtop = 0;
	SliderArrtop = 0;
}

void TradeScreen::Update() {

}