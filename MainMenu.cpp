#include "ScreenList.h"

MainMenu::MainMenu(SDL_Renderer* r, int Width, int Height, std::function<void()> fp, std::function<void(Screen*)> fpl) : Screen(r, Width, Height) {
	bHasBackground = true;
	SetupBg("Backgrounds/OldMenu.png");

	int fontSize = 32;
	auto change = std::bind(&MainMenu::StartGame, this);

	ButtonArr[0] = new Button(r, int(Width * 0.22), int(Height * 0.2), int(Width * 0.12), int(Height * 0.06), "New Game", fontSize, change);
	ButtonArr[1] = new Button(r, int(Width * 0.22), int(Height * 0.3), int(Width * 0.12), int(Height * 0.06), "Load Game", fontSize, fp);

	change = std::bind(&MainMenu::ShowSettings, this);

	ButtonArr[2] = new Button(r, int(Width * 0.23), int(Height * 0.4), int(Width * 0.10), int(Height * 0.06), "Settings", fontSize, change);

	change = std::bind(&MainMenu::ShowCredits, this);

	ButtonArr[3] = new Button(r, int(Width * 0.235), int(Height * 0.5), int(Width * 0.09), int(Height * 0.06), "Credits", fontSize, change);
	ButtonArr[4] = new Button(r, int(Width * 0.24), int(Height * 0.6), int(Width * 0.08), int(Height * 0.06), "Quit", fontSize, fp);

	ButtonArrtop = 5;
	LabelArrtop = 0;
	ImageArrtop = 0;
	SliderArrtop = 0;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void MainMenu::ShowCredits(){
	CreditScreen* CS = new CreditScreen(renderer, WindowSize[0], WindowSize[1], QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(CS);
}

void MainMenu::ShowSettings() {
	MenuSettingsScreen* CS = new MenuSettingsScreen(renderer, WindowSize[0], WindowSize[1], QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(CS);
}

void MainMenu::StartGame() {
	CountrySelection* GS = new CountrySelection(renderer, WindowSize[0], WindowSize[1], QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(GS);
}