#include "ScreenList.h"
#include "MainWindow.h"
#include "MessageBox.h"
#include "Button.h"

MainMenu::MainMenu(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : Screen(mw, fp, fpl) {
	SetupBg("Backgrounds/OldMenu.png");
        auto [Width, Height] = mw.GetWindowDimensions();

	int fontSize = 32;

	AddDrawable<Button>(mw, int(Width * 0.22), int(Height * 0.2), int(Width * 0.12), int(Height * 0.06), "New Game", fontSize, [this]{ StartGame(); });
	AddDrawable<Button>(mw, int(Width * 0.22), int(Height * 0.3), int(Width * 0.12), int(Height * 0.06), "Load Game", fontSize, fp);

	AddDrawable<Button>(mw, int(Width * 0.22), int(Height * 0.4), int(Width * 0.12), int(Height * 0.06), "Settings", fontSize, [this]{ ShowSettings(); });

	AddDrawable<Button>(mw, int(Width * 0.22), int(Height * 0.5), int(Width * 0.12), int(Height * 0.06), "Credits", fontSize, [this]{ ShowCredits(); });
	AddDrawable<Button>(mw, int(Width * 0.22), int(Height * 0.6), int(Width * 0.12), int(Height * 0.06), "Quit", fontSize, fp);

        AddMessageBox<MessageBox>(mw, "Welcome to The Great War", "Hello dear player, \n"
                "We just want to inform you that the game is still in a beta stage and will certainly have some bugs. Please be tolerant.\n"
                "Kind regards,\n"
                "The Great War devs.", [this](void* p) { DeleteMessageBox(p);} );
}

void MainMenu::ShowCredits(){
	ChangeScreenFunc(std::make_unique<CreditScreen>(*main_window, QuitFunc, ChangeScreenFunc));
}

void MainMenu::ShowSettings() {
	ChangeScreenFunc(std::make_unique<MenuSettingsScreen>(*main_window, QuitFunc, ChangeScreenFunc));
}

void MainMenu::StartGame() {
	ChangeScreenFunc(std::make_unique<CountrySelection>(*main_window, QuitFunc, ChangeScreenFunc));
}
