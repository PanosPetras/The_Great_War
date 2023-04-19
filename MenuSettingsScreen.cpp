#include "ScreenList.h"

#include "Button.h"
#include "Checkbox.h"
#include "MainWindow.h"

MenuSettingsScreen::MenuSettingsScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : BackScreen(mw, fp, fpl) {
	SetupBg("Backgrounds/OldMenu.png");
    auto [Width, Height] = mw.GetWindowDimensions();

	AddDrawable<Button>(mw, int(Width * 0.75), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Apply", 32, [this] { ApplyChanges(); }, SDLK_RETURN);
	AddDrawable<Button>(mw, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, [this]{ Back(); }, SDLK_ESCAPE);
	AddDrawable<Button>(mw, int(Width * 0.18), int(Height * 0.2), int(Width * 0.02), int(Height * 0.06), "<", 32);
	AddLabel<Label>(mw, "1920x1080", int(Height / 33.75), int(Width * 0.21), int(Height * 0.23), center_left);
	AddDrawable<Button>(mw, int(Width * 0.28), int(Height * 0.2), int(Width * 0.02), int(Height * 0.06), ">", 32);
	//AddDrawable<Checkbox>(mw, int(Width * 0.18), int(Height * 0.3), int(Height * 0.06), "VSync", 32);
}

void MenuSettingsScreen::ApplyChanges() {
	//SDL_SetWindowSize();
	//SDL_RenderSetVSync()
}
