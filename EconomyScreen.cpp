#include "ScreenList.h"


EconomyScreen::EconomyScreen(SDL_Renderer* r, int Width, int Height, std::function<void()> fp, std::function<void(Screen*)> fpl) : Screen(r, Width, Height) {
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");

	LabelArrtop = 0;
	ImageArrtop = 0;
	ButtonArrtop = 0;
	SliderArrtop = 0;
}