#include "ScreenList.h"

MenuSettingsScreen::MenuSettingsScreen(SDL_Renderer* r, int Width, int Height, std::function<void()> fp, std::function<void(Screen*)> fpl) : Screen(r, Width, Height) {
	bHasBackground = true;
	SetupBg("Backgrounds/OldMenu.png");
	auto change = std::bind(&MenuSettingsScreen::Back, this);
	ButtonArr[0] = new Button(r, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, change, SDLK_ESCAPE);
	ButtonArrtop = 1;
	LabelArrtop = 0;
	ImageArrtop = 0;
	SliderArrtop = 0;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void MenuSettingsScreen::Back() {
	MainMenu* CS = new MainMenu(renderer, WindowSize[0], WindowSize[1], QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(CS);
}