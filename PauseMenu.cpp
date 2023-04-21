#include "Button.h"
#include "Image.h"
#include "MainWindow.h"
#include "ScreenList.h"

PauseMenu::PauseMenu(MainWindow& mw, std::function<void()> fp, std::function<void()> UnpauseF, std::function<void(std::unique_ptr<Screen>)> fpl) : Screen(mw) {
    auto [Width, Height] = mw.GetWindowDimensions();

    int fontSize = 32;
    auto change = std::bind(&PauseMenu::ReturnToMainMenu, this);

    AddImage<Image>(mw, "Backgrounds/Pause_Menu.png", int(Width * 0.25), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));
    AddDrawable<Button>(mw, int(Width * 0.34), int(Height * 0.35), int(Width * 0.12), int(Height * 0.06), "Back", fontSize, UnpauseF);
    AddDrawable<Button>(mw, int(Width * 0.34), int(Height * 0.45), int(Width * 0.12), int(Height * 0.06), "Save Game", fontSize);
    AddDrawable<Button>(mw, int(Width * 0.34), int(Height * 0.55), int(Width * 0.12), int(Height * 0.06), "Resign", fontSize, change);
    AddDrawable<Button>(mw, int(Width * 0.34), int(Height * 0.65), int(Width * 0.12), int(Height * 0.06), "Quit", fontSize, fp);

    bHasBackground = false;
    ChangeScreenFunc = fpl;
    QuitFunc = fp;
}

void PauseMenu::ReturnToMainMenu() {
    ChangeScreenFunc(std::make_unique<MainMenu>(*main_window, QuitFunc, ChangeScreenFunc));
}
