#include "ScreenList.h"

DiplomacyScreen::DiplomacyScreen(SDL_Renderer* r, int Width, int Height, PlayerController* PC) : Screen(r, Width, Height){
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");

	ImageArr[0] = new Image(r, "Backgrounds/old_paper.png", int(Width * 0.4), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));

	ImageArr[1] = new Image(r, std::string("Flags/") + PC->player_tag + ".png", int(Width * 0.4), int(Height * 0.2), 120, 80);

	LabelArr[0] = new Label(r, PC->CountriesArr[PC->player_index]->name, 32, int(Width * 0.48), int(Height * 0.218));

	ButtonArrtop = CreateCountryButtons(PC);

	int fontSize = 26;
	ButtonArr[ButtonArrtop++] = new Button(r, int(Width * 0.435), int(Height * 0.4), int(Width * 0.11), int(Height * 0.04), "Declare War", fontSize);
	ButtonArr[ButtonArrtop++] = new Button(r, int(Width * 0.43), int(Height * 0.48), int(Width * 0.14), int(Height * 0.04), "Propose Alliance", fontSize);

	LabelArrtop = 1;
	ImageArrtop = 2;
	SliderArrtop = 0;

	PCref = PC;
}

DiplomacyScreen::DiplomacyScreen(SDL_Renderer* r, int Width, int Height, int index, PlayerController* PC) : DiplomacyScreen(r, Width, Height, PC){
	SelectCountry((void*)((Uint64)index));
}

int DiplomacyScreen::CreateCountryButtons(PlayerController* PC) {
	int i = 0, flagsPerLine = 12;

	for (auto country : PC->CountriesArr) {
		std::function<void(void*)> func = std::bind(&DiplomacyScreen::SelectCountry, this, std::placeholders::_1);
		ButtonArr[i] = new Button(renderer, int(WindowSize[0] * (0.1 + (i / flagsPerLine) * 0.05)), int(WindowSize[1] * (0.17 + (i % flagsPerLine) * 0.06)), 60, 40, ("Flags/" + country->tag).c_str(), func, (void*)((Uint64)i));
		i++;
	}

	return i;
}

void DiplomacyScreen::SelectCountry(void* country){
	int index = (int)(Uint64)(country);

	ImageArr[1]->ChangeImage(std::string("Flags/") + PCref->CountriesArr[index]->tag + ".png");
	LabelArr[0]->ChangeText(PCref->CountriesArr[index]->name);
}
