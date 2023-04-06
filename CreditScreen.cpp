#include "ScreenList.h"
#include "WindowInfo.h"
#include "Button.h"
#include "Label.h"

CreditScreen::CreditScreen(SDL_Renderer* r, std::function<void()> fp, std::function<void(Screen*)> fpl) : Screen(r) {
	bHasBackground = true;
	SetupBg("Backgrounds/OldMenu.png");

	int fontSize = 40, Width = GetWindowWidth(), Height = GetWindowHeight();
	auto change = std::bind(&CreditScreen::Back, this);

	InputDrawableArr[0] = new Button(r, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, change, SDLK_ESCAPE);
	LabelArr[0] = new Label(r, "Backgrounds are paintings from the US national war museum.", fontSize, int(Width * 0.1), int(Height * 0.2));
	LabelArr[1] = new Label(r, "The icons used were downloaded from flaticon. Made by users : srip, freepik, smashicons.", fontSize, int(Width * 0.1), int(Height * 0.3));
	LabelArr[2] = new Label(r, "Graphics library: SDL 2.0", fontSize, int(Width * 0.1), int(Height * 0.4));
	LabelArr[3] = new Label(r, "The map used was created by user tyo and uploaded on the civfanatics forum.", fontSize, int(Width * 0.1), int(Height * 0.5));

	InputDrawableArrtop = 1;
	LabelArrtop = 4;
	ImageArrtop = 0;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void CreditScreen::Back() {
	MainMenu* CS = new MainMenu(renderer, QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(CS);
}