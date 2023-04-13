#include "ScreenList.h"
#include "MainWindow.h"
#include "Button.h"
#include "Image.h"

PauseMenu::PauseMenu(SDL_Renderer* r, std::function<void()> fp, std::function<void()> UnpauseF, std::function<void(std::unique_ptr<Screen>)> fpl) : Screen(r) {
    auto [Width, Height] = GetWindowDimensions();

    int fontSize = 32;
    auto change = std::bind(&PauseMenu::ReturnToMainMenu, this);

    AddImage<Image>(renderer, "Backgrounds/Pause_Menu.png", int(Width * 0.25), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));
    AddDrawable<Button>(r, int(Width * 0.34), int(Height * 0.35), int(Width * 0.12), int(Height * 0.06), "Back", fontSize, UnpauseF);
    AddDrawable<Button>(r, int(Width * 0.34), int(Height * 0.45), int(Width * 0.12), int(Height * 0.06), "Save Game", fontSize);
    AddDrawable<Button>(r, int(Width * 0.34), int(Height * 0.55), int(Width * 0.12), int(Height * 0.06), "Resign", fontSize, change);
    AddDrawable<Button>(r, int(Width * 0.34), int(Height * 0.65), int(Width * 0.12), int(Height * 0.06), "Quit", fontSize, fp);

    bHasBackground = false;
    ChangeScreenFunc = fpl;
    QuitFunc = fp;
}

void PauseMenu::ReturnToMainMenu(){
    ChangeScreenFunc(std::make_unique<MainMenu>(renderer, QuitFunc, ChangeScreenFunc));
}
