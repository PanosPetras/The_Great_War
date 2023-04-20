#include "ScreenList.h"

#include "Button.h"
#include "Checkbox.h"
#include "MainWindow.h"
#include "Resolutions.h"
#include <array>

MenuSettingsScreen::MenuSettingsScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : BackScreen(mw, fp, fpl) {
    SetupBg("Backgrounds/OldMenu.png");
    auto [Width, Height] = mw.GetWindowDimensions();
    int fontSize = int(Height / 33.75);

    auto res = Resolutions::Resolution(mw.Width(), mw.Height());
    currentResolutionIndex = Resolutions::findResolutionIndex(res);

    AddDrawable<Button>(mw, int(Width * 0.75), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Apply", fontSize, [this] { ApplyChanges(); }, SDLK_RETURN);
    AddDrawable<Button>(mw, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", fontSize, [this]{ Back(); }, SDLK_ESCAPE);
    AddDrawable<Button>(mw, int(Width * 0.18), int(Height * 0.2), int(Width * 0.02), int(Height * 0.06), "<", fontSize, [this] {DecreaseResolution(); });
    AddLabel<Label>(mw, res.toString(), fontSize, int(Width * 0.25), int(Height * 0.23), center);
    AddDrawable<Button>(mw, int(Width * 0.3), int(Height * 0.2), int(Width * 0.02), int(Height * 0.06), ">", fontSize, [this] {IncreaseResolution(); });
    AddDrawable<Checkbox>(mw, (int)(Width * 0.18), (int)(Height * 0.3), (int)(Height * 0.06), "VSync", fontSize);
    As<Checkbox>(InputDrawableArrtop() - 1).ChangeValue(main_window->vsync);
    AddDrawable<Checkbox>(mw, (int)(Width * 0.18), (int)(Height * 0.4), (int)(Height * 0.06), "Fullscreen", fontSize);
    As<Checkbox>(InputDrawableArrtop() - 1).ChangeValue(main_window->fullscreen);
}

void MenuSettingsScreen::UpdateResolutionLabel() {
    LabelArr[0]->ChangeText(Resolutions::SUPPORTED_RESOLUTIONS[currentResolutionIndex].toString());
}

void MenuSettingsScreen::IncreaseResolution() {
    if (currentResolutionIndex < Resolutions::SUPPORTED_RESOLUTIONS.size() - 1) {
        currentResolutionIndex++;
    }

    UpdateResolutionLabel();
}

void MenuSettingsScreen::DecreaseResolution() {
    if (currentResolutionIndex > 0) {
        currentResolutionIndex--;
    }

    UpdateResolutionLabel();
}

void MenuSettingsScreen::ApplyChanges() {
    auto res = Resolutions::SUPPORTED_RESOLUTIONS[currentResolutionIndex];
    main_window->vsync = As<Checkbox>(InputDrawableArrtop() - 2).GetValue();
    main_window->fullscreen = As<Checkbox>(InputDrawableArrtop() - 1).GetValue();

    SDL_SetWindowFullscreen(main_window->window, main_window->fullscreen);
    SDL_RenderSetVSync(*main_window, main_window->vsync);
    SDL_SetWindowSize(main_window->window, res.GetWidth(), res.GetHeight());

    SDL_GetWindowSize(main_window->window, &main_window->windim.x, &main_window->windim.y);

    ChangeScreenFunc(std::make_unique<MenuSettingsScreen>(*main_window, QuitFunc, ChangeScreenFunc));
}
