#include "ScreenList.h"

MainMenu::MainMenu(SDL_Renderer* r, int Width, int Height, std::function<void()> fp, std::function<void(Screen*)> fpl) {
	renderer = r;
	bHasBackground = true;
	SetupBg("Backgrounds/test.png");
	auto change = std::bind(&MainMenu::StartGame, this);
	ButtonArr[0] = new Button(r, int(Width * 0.22), int(Height * 0.2), int(Width * 0.12), int(Height * 0.06), "Buttons/Menus/New_Game", change);
	ButtonArr[1] = new Button(r, int(Width * 0.22), int(Height * 0.3), int(Width * 0.12), int(Height * 0.06), "Buttons/Menus/Load_Game", fp);
	ButtonArr[2] = new Button(r, int(Width * 0.23), int(Height * 0.4), int(Width * 0.10), int(Height * 0.06), "Buttons/Menus/Settings", fp);
	change = std::bind(&MainMenu::ShowCredits, this);
	ButtonArr[3] = new Button(r, int(Width * 0.235), int(Height * 0.5), int(Width * 0.09), int(Height * 0.06), "Buttons/Menus/Credits", change);
	ButtonArr[4] = new Button(r, int(Width * 0.24), int(Height * 0.6), int(Width * 0.08), int(Height * 0.06), "Buttons/Menus/Quit", fp);
	ButtonArrtop = 5;
	LabelArrtop = 0;
	ImageArrtop = 0;
	SliderArrtop = 0;
	WindowSize[0] = Width;
	WindowSize[1] = Height;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void MainMenu::ShowCredits(){
	CreditScreen* CS = new CreditScreen(renderer, WindowSize[0], WindowSize[1], this->ButtonArr[4]->func, ChangeScreenFunc);
	ChangeScreenFunc(CS);
}

void MainMenu::StartGame() {
	CountrySelection* GS = new CountrySelection(renderer, WindowSize[0], WindowSize[1], QuitFunc, ChangeScreenFunc);
	//GameScreen* GS = new GameScreen(renderer, WindowSize[1], WindowSize[0], "kuk", QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(GS);
}