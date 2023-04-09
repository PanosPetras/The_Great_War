#include "ScreenList.h"
#include "WindowInfo.h"
#include "Button.h"

MenuSettingsScreen::MenuSettingsScreen(SDL_Renderer* r, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : Screen(r) {
	bHasBackground = true;
	SetupBg("Backgrounds/OldMenu.png");

	int Width = GetWindowWidth(), Height = GetWindowHeight();
	auto change = std::bind(&MenuSettingsScreen::Back, this);

	AddDrawable<Button>(r, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, change, SDLK_ESCAPE);

	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void MenuSettingsScreen::Back() {
	ChangeScreenFunc(std::make_unique<MainMenu>(renderer, QuitFunc, ChangeScreenFunc));
}
