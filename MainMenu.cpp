#include "ScreenList.h"
#include "WindowInfo.h"
#include "MessageBox.h"

MainMenu::MainMenu(SDL_Renderer* r, std::function<void()> fp, std::function<void(Screen*)> fpl) : Screen(r) {
	bHasBackground = true;
	SetupBg("Backgrounds/OldMenu.png");

	int fontSize = 32, Width = GetWindowWidth(), Height = GetWindowHeight();
	auto change = std::bind(&MainMenu::StartGame, this);

	InputDrawableArr[0] = new Button(r, int(Width * 0.22), int(Height * 0.2), int(Width * 0.12), int(Height * 0.06), "New Game", fontSize, change);
	InputDrawableArr[1] = new Button(r, int(Width * 0.22), int(Height * 0.3), int(Width * 0.12), int(Height * 0.06), "Load Game", fontSize, fp);

	change = std::bind(&MainMenu::ShowSettings, this);

	InputDrawableArr[2] = new Button(r, int(Width * 0.22), int(Height * 0.4), int(Width * 0.12), int(Height * 0.06), "Settings", fontSize, change);

	change = std::bind(&MainMenu::ShowCredits, this);

	InputDrawableArr[3] = new Button(r, int(Width * 0.22), int(Height * 0.5), int(Width * 0.12), int(Height * 0.06), "Credits", fontSize, change);
	InputDrawableArr[4] = new Button(r, int(Width * 0.22), int(Height * 0.6), int(Width * 0.12), int(Height * 0.06), "Quit", fontSize, fp);
	InputDrawableArr[5] = new MessageBox(r, "Hello", "Lorem Ipsum is simply dummy text of the printing and typesetting industry.\r Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book.");

	InputDrawableArrtop = 6;
	LabelArrtop = 0;
	ImageArrtop = 0;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void MainMenu::ShowCredits(){
	CreditScreen* CS = new CreditScreen(renderer, QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(CS);
}

void MainMenu::ShowSettings() {
	MenuSettingsScreen* CS = new MenuSettingsScreen(renderer, QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(CS);
}

void MainMenu::StartGame() {
	CountrySelection* GS = new CountrySelection(renderer, QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(GS);
}