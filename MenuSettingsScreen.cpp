#include "ScreenList.h"

#include "Button.h"
#include "MainWindow.h"

MenuSettingsScreen::MenuSettingsScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : BackScreen(mw, fp, fpl) {
	SetupBg("Backgrounds/OldMenu.png");
        auto [Width, Height] = mw.GetWindowDimensions();

	AddDrawable<Button>(mw, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, [this]{ Back(); }, SDLK_ESCAPE);
}
