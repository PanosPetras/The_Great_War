#include "ScreenList.h"

#include "Button.h"
#include "Label.h"
#include "MainWindow.h"

CreditScreen::CreditScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : BackScreen(mw, fp, fpl) {
	SetupBg("Backgrounds/OldMenu.png");

	int fontSize = 40;
        auto [Width, Height] = mw.GetWindowDimensions();

	AddDrawable<Button>(*main_window, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, [this]{ Back(); }, SDLK_ESCAPE);

	AddLabel<Label>(*main_window,
                        "Backgrounds are paintings from the US national war museum.\n"
                        "The icons used were downloaded from flaticon. Made by users : srip, freepik, smashicons.\n"
                        "Graphics library: SDL 2.0\n"
                        "The map used was created by user tyo and uploaded on the civfanatics forum.",
                        fontSize, int(Width * 0.1), int(Height * 0.2), int(Width * 0.8));
}
