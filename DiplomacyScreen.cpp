#include "ScreenList.h"
#include <iostream>

DiplomacyScreen::DiplomacyScreen(SDL_Renderer* r, int Width, int Height, PlayerController* PC) : Screen(r, Width, Height){
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");
	ButtonArrtop = CreateCountryButtons(PC);
	LabelArrtop = 0;
	ImageArrtop = 0;
	SliderArrtop = 0;

	PCref = PC;
}

int DiplomacyScreen::CreateCountryButtons(PlayerController* PC) {
	int i = 0, flagsPerLine = 12;
	std::cout << PC->CountriesArr.size();
	for (auto country : PC->CountriesArr) {
		ButtonArr[i] = new Button(renderer, int(WindowSize[0] * (0.1 + (i / flagsPerLine) * 0.05)), int(WindowSize[1] * (0.17 + (i % flagsPerLine) * 0.06)), 60, 40, ("Flags/" + country->countrytag).c_str());
		i++;
	}
	return i;
}