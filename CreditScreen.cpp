#include "ScreenList.h"
#include "WindowInfo.h"
#include "Button.h"
#include "Label.h"

CreditScreen::CreditScreen(SDL_Renderer* r, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : Screen(r) {
	bHasBackground = true;
	SetupBg("Backgrounds/OldMenu.png");

	int fontSize = 40, Width = GetWindowWidth(), Height = GetWindowHeight();
	auto change = std::bind(&CreditScreen::Back, this);

	AddDrawable<Button>(r, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, change, SDLK_ESCAPE);
	AddLabel<Label>(r, "Backgrounds are paintings from the US national war museum.\nThe icons used were downloaded from flaticon. Made by users : srip, freepik, smashicons.\nGraphics library: SDL 2.0\nThe map used was created by user tyo and uploaded on the civfanatics forum.", fontSize, int(Width * 0.1), int(Height * 0.2), int(Width * 0.8));

	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void CreditScreen::Back() {
	ChangeScreenFunc(std::make_unique<MainMenu>(renderer, QuitFunc, ChangeScreenFunc));
}
