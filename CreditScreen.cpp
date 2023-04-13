#include "ScreenList.h"

#include "Button.h"
#include "Label.h"
#include "MainWindow.h"

CreditScreen::CreditScreen(SDL_Renderer_ctx& r, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : BackScreen(r, fp, fpl) {
	SetupBg("Backgrounds/OldMenu.png");

	int fontSize = 40;
        auto [Width, Height] = GetWindowDimensions();

	AddDrawable<Button>(r, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, [this]{ Back(); }, SDLK_ESCAPE);
	AddLabel<Label>(r, "Backgrounds are paintings from the US national war museum.\nThe icons used were downloaded from flaticon. Made by users : srip, freepik, smashicons.\nGraphics library: SDL 2.0\nThe map used was created by user tyo and uploaded on the civfanatics forum.", fontSize, int(Width * 0.1), int(Height * 0.2), int(Width * 0.8));
}
