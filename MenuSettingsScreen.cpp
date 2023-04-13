#include "ScreenList.h"

#include "Button.h"
#include "MainWindow.h"

MenuSettingsScreen::MenuSettingsScreen(SDL_Renderer* r, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : BackScreen(r, fp, fpl) {
	SetupBg("Backgrounds/OldMenu.png");
        auto [Width, Height] = GetWindowDimensions();

	AddDrawable<Button>(r, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, [this]{ Back(); }, SDLK_ESCAPE);
}
