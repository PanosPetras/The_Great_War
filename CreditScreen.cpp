#include "ScreenList.h"

CreditScreen::CreditScreen(SDL_Renderer* r, int Width, int Height, std::function<void()> fp, std::function<void(Screen*)> fpl) : Screen(r, Width, Height) {
	bHasBackground = true;
	SetupBg("Backgrounds/main_menu.png");
	auto change = std::bind(&CreditScreen::Back, this);
	ButtonArr[0] = new Button(r, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Buttons/Menus/Back", change, SDLK_ESCAPE);
	LabelArr[0] = new Label(r, "Backgrounds are paintings from the US national war museum.", 40, int(Width * 0.1), int(Height * 0.2));
	LabelArr[1] = new Label(r, "The icons used were downloaded from flaticon. Made by users : srip, freepik, smashicons.", 40, int(Width * 0.1), int(Height * 0.3));
	LabelArr[2] = new Label(r, "Graphics library: SDL 2.0", 40, int(Width * 0.1), int(Height * 0.4));
	LabelArr[3] = new Label(r, "The map used was created by user tyo and uploaded on the civfanatics forum.", 40, int(Width * 0.1), int(Height * 0.5));
	ButtonArrtop = 1;
	LabelArrtop = 4;
	ImageArrtop = 0;
	SliderArrtop = 0;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void CreditScreen::Back() {
	MainMenu* CS = new MainMenu(renderer, WindowSize[0], WindowSize[1], QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(CS);
}