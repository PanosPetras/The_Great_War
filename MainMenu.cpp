#include "ScreenList.h"
#include "WindowInfo.h"
#include "MessageBox.h"
#include "Button.h"

MainMenu::MainMenu(SDL_Renderer* r, std::function<void()> fp, std::function<void(Screen*)> fpl) : Screen(r) {
	bHasBackground = true;
	SetupBg("Backgrounds/OldMenu.png");

	int fontSize = 32, Width = GetWindowWidth(), Height = GetWindowHeight();

	InputDrawableArr[0] = new Button(r, int(Width * 0.22), int(Height * 0.2), int(Width * 0.12), int(Height * 0.06), "New Game", fontSize, [this]{StartGame();});
	InputDrawableArr[1] = new Button(r, int(Width * 0.22), int(Height * 0.3), int(Width * 0.12), int(Height * 0.06), "Load Game", fontSize, fp);

	InputDrawableArr[2] = new Button(r, int(Width * 0.22), int(Height * 0.4), int(Width * 0.12), int(Height * 0.06), "Settings", fontSize, [this]{ShowSettings();});

	InputDrawableArr[3] = new Button(r, int(Width * 0.22), int(Height * 0.5), int(Width * 0.12), int(Height * 0.06), "Credits", fontSize, [this]{ShowCredits();});
	InputDrawableArr[4] = new Button(r, int(Width * 0.22), int(Height * 0.6), int(Width * 0.12), int(Height * 0.06), "Quit", fontSize, fp);

	messageBoxes.push_back(new MessageBox(r, "Welcome to The Great War", "Hello dear player, \nWe just want to inform you that the game is still in a beta stage and will certainly have some bugs. Please be tolerant.\nKind regards, \nThe Great War devs.", [this](void* p) { DeleteMessageBox(p);} ));

	InputDrawableArrtop = 5;
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
