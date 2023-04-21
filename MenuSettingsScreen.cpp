#include "ScreenList.h"

#include "Button.h"
#include "Checkbox.h"
#include "MainWindow.h"
#include "Resolutions.h"
#include <array>

MenuSettingsScreen::MenuSettingsScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) :
    BackScreen(mw, fp, fpl) {
    SetupBg("Backgrounds/OldMenu.png");
    auto [Width, Height] = mw.GetWindowDimensions();
    int fontSize = int(Height / 33.75);

    auto res = Resolutions::Resolution(mw.Width(), mw.Height());
    currentResolutionIndex = Resolutions::findResolutionIndex(res);
    currentFramerateIndex = Resolutions::findFramerateIndex(main_window->framerateCap);

    AddDrawable<Checkbox>(mw, (int)(Width * 0.18), (int)(Height * 0.2), (int)(Height * 0.06), "Fullscreen", fontSize,
                          [this](bool state) { onFullscreenToggle(state); });
    As<Checkbox>(0).ChangeValue(main_window->fullscreen);

    AddDrawable<Button>(mw, int(Width * 0.18), int(Height * 0.3), int(Width * 0.02), int(Height * 0.06), "<", fontSize,
                        [this] { DecreaseResolution(); });
    AddLabel<Label>(mw, res.toString(), fontSize, int(Width * 0.25), int(Height * 0.33), center);
    AddDrawable<Button>(mw, int(Width * 0.3), int(Height * 0.3), int(Width * 0.02), int(Height * 0.06), ">", fontSize,
                        [this] { IncreaseResolution(); });
    onFullscreenToggle(main_window->fullscreen);

    AddDrawable<Checkbox>(mw, (int)(Width * 0.18), (int)(Height * 0.4), (int)(Height * 0.06), "VSync", fontSize,
                          [this](bool state) { onVSyncToggle(state); });
    As<Checkbox>(3).ChangeValue(main_window->vsync);

    AddDrawable<Button>(mw, int(Width * 0.18), int(Height * 0.5), int(Width * 0.02), int(Height * 0.06), "<", fontSize,
                        [this] { DecreaseFramerate(); });
    AddLabel<Label>(mw, std::to_string(Resolutions::SUPPORTED_FRAMERATES[currentFramerateIndex]), fontSize, int(Width * 0.25),
                    int(Height * 0.53), center);
    AddDrawable<Button>(mw, int(Width * 0.3), int(Height * 0.5), int(Width * 0.02), int(Height * 0.06), ">", fontSize,
                        [this] { IncreaseFramerate(); });
    onVSyncToggle(main_window->vsync);

    AddDrawable<Button>(
        mw, int(Width * 0.75), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Apply", fontSize,
        [this] { ApplyChanges(); }, SDLK_RETURN);
    AddDrawable<Button>(
        mw, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", fontSize, [this] { Back(); },
        SDLK_ESCAPE);
}

void MenuSettingsScreen::UpdateResolutionLabel() {
    LabelArr[0]->ChangeText(Resolutions::SUPPORTED_RESOLUTIONS[currentResolutionIndex].toString());
}

void MenuSettingsScreen::IncreaseResolution() {
    if(currentResolutionIndex < Resolutions::SUPPORTED_RESOLUTIONS.size() - 1) {
        currentResolutionIndex++;
    }

    UpdateResolutionLabel();
}

void MenuSettingsScreen::DecreaseResolution() {
    if(currentResolutionIndex > 0) {
        currentResolutionIndex--;
    }

    UpdateResolutionLabel();
}

void MenuSettingsScreen::UpdateFramerateLabel() {
    LabelArr[1]->ChangeText(std::to_string(Resolutions::SUPPORTED_FRAMERATES[currentFramerateIndex]));
}

void MenuSettingsScreen::IncreaseFramerate() {
    if(currentFramerateIndex < Resolutions::SUPPORTED_FRAMERATES.size() - 1) {
        currentFramerateIndex++;
    }

    UpdateFramerateLabel();
}

void MenuSettingsScreen::DecreaseFramerate() {
    if(currentFramerateIndex > 0) {
        currentFramerateIndex--;
    }

    UpdateFramerateLabel();
}

void MenuSettingsScreen::onVSyncToggle(bool state) {
    As<Button>(4).SetActive(!state);
    As<Button>(5).SetActive(!state);

    if(state) {
        LabelArr[1]->ChangeColor({70, 70, 70});
    } else {
        LabelArr[1]->ChangeColor({0, 0, 0});
    }
}

void MenuSettingsScreen::onFullscreenToggle(bool state) {
    As<Button>(1).SetActive(!state);
    As<Button>(2).SetActive(!state);

    if(state) {
        LabelArr[0]->ChangeColor({70, 70, 70});
    } else {
        LabelArr[0]->ChangeColor({0, 0, 0});
    }
}

void MenuSettingsScreen::ApplyChanges() {
    auto res = Resolutions::SUPPORTED_RESOLUTIONS[currentResolutionIndex];
    main_window->vsync = As<Checkbox>(3).GetValue();
    main_window->fullscreen = As<Checkbox>(0).GetValue();
    main_window->framerateCap = Resolutions::SUPPORTED_FRAMERATES[currentFramerateIndex];

    SDL_SetWindowFullscreen(main_window->window, main_window->fullscreen);
    SDL_RenderSetVSync(*main_window, main_window->vsync);
    SDL_SetWindowSize(main_window->window, res.GetWidth(), res.GetHeight());

    SDL_GetWindowSize(main_window->window, &main_window->windim.x, &main_window->windim.y);

    ChangeScreenFunc(std::make_unique<MenuSettingsScreen>(*main_window, QuitFunc, ChangeScreenFunc));
}
