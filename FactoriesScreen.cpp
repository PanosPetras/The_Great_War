#include "ScreenList.h"


FactoriesScreen::FactoriesScreen(SDL_Renderer* r, int Width, int Height, std::vector<std::string> StatesNames, std::vector<std::string> Factories, std::function<void()> fp, std::function<void(Screen*)> fpl) {
	renderer = r;
	WindowSize[0] = Width;
	WindowSize[1] = Height;
}