#include "ScreenList.h"
#include "WindowInfo.h"
#include "Button.h"

MenuSettingsScreen::MenuSettingsScreen(SDL_Renderer* r, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : BackScreen(r, fp, fpl) {
	SetupBg("Backgrounds/OldMenu.png");

	int Width = GetWindowWidth(), Height = GetWindowHeight();

	AddDrawable<Button>(r, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, [this]{ Back(); }, SDLK_ESCAPE);
}
