#include "ScreenList.h"
#include "WindowInfo.h"

MenuSettingsScreen::MenuSettingsScreen(SDL_Renderer* r, std::function<void()> fp, std::function<void(Screen*)> fpl) : Screen(r) {
	bHasBackground = true;
	SetupBg("Backgrounds/OldMenu.png");

	int Width = GetWindowWidth(), Height = GetWindowHeight();
	auto change = std::bind(&MenuSettingsScreen::Back, this);

	InputDrawableArr[0] = new Button(r, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, change, SDLK_ESCAPE);

	InputDrawableArrtop = 1;
	LabelArrtop = 0;
	ImageArrtop = 0;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void MenuSettingsScreen::Back() {
	MainMenu* CS = new MainMenu(renderer, QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(CS);
}