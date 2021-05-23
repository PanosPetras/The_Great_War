#include "ScreenList.h"


EconomyScreen::EconomyScreen(SDL_Renderer* r, int Width, int Height, std::function<void()> fp, std::function<void(Screen*)> fpl) {
	renderer = r;
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");
	WindowSize[0] = Width;
	WindowSize[1] = Height;

	LabelArrtop = 0;
	ImageArrtop = 0;
	ButtonArrtop = 0;
}