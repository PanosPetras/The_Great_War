#include "ScreenList.h"
#include "WindowInfo.h"
#include "Button.h"
#include "Image.h"

PauseMenu::PauseMenu(SDL_Renderer* r, std::function<void()> fp, std::function<void()> UnpauseF, std::function<void(Screen*)> fpl) : Screen(r) {
	bHasBackground = false;

	int fontSize = 32, Width = GetWindowWidth(), Height = GetWindowHeight();
	auto change = std::bind(&PauseMenu::ReturnToMainMenu, this);

	ImageArr[0] = new Image(renderer, "Backgrounds/Pause_Menu.png", int(Width * 0.25), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));
	InputDrawableArr[0] = new Button(r, int(Width * 0.34), int(Height * 0.35), int(Width * 0.12), int(Height * 0.06), "Back", fontSize, UnpauseF);
	InputDrawableArr[1] = new Button(r, int(Width * 0.34), int(Height * 0.45), int(Width * 0.12), int(Height * 0.06), "Save Game", fontSize);
	InputDrawableArr[2] = new Button(r, int(Width * 0.34), int(Height * 0.55), int(Width * 0.12), int(Height * 0.06), "Resign", fontSize, change);
	InputDrawableArr[3] = new Button(r, int(Width * 0.34), int(Height * 0.65), int(Width * 0.12), int(Height * 0.06), "Quit", fontSize, fp);

	InputDrawableArrtop = 4;
	LabelArrtop = 0;
	ImageArrtop = 1;
	bHasBackground = false;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void PauseMenu::ReturnToMainMenu(){
	MainMenu* MM = new MainMenu(renderer, QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(MM);
}