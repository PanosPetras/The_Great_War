#include "ScreenList.h"


FactoriesScreen::FactoriesScreen(SDL_Renderer* r, int Width, int Height, std::vector<std::string> StatesNames, std::vector<std::vector<std::string>> Factories, std::function<void()> fp, std::function<void(Screen*)> fpl) {
	renderer = r;
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");
	WindowSize[0] = Width;
	WindowSize[1] = Height;

	LabelArr[0] = new Label(r, StatesNames[0].c_str() , 32, 300, 300);
	LabelArrtop = 1;
	ImageArrtop = 0;
	ButtonArrtop = 0;
}