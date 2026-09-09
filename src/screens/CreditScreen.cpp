#include "screens/ScreenList.h"

#include "ui/Button.h"
#include "ui/Label.h"
#include "core/MainWindow.h"

CreditScreen::CreditScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : BackScreen(mw, fp, fpl) {
    SetupBg("Backgrounds/OldMenu.png");

    auto [Width, Height] = mw.GetWindowDimensions();
    constexpr auto fontSize = FontSize::Title;

    AddDrawable<Button>(
        *main_window, int(Width * 0.85), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", FontSize::Heading, [this] { Back(); }, SDLK_ESCAPE);

    AddLabel<Label>(*main_window,
                    "Backgrounds are paintings from the US national war museum.\n"
                    "The icons used were downloaded from flaticon. Made by users : srip, freepik, smashicons.\n"
                    "Graphics library: SDL 2.0\n"
                    "The map used was created by user tyo and uploaded on the civfanatics forum.",
                    fontSize, int(Width * 0.1), int(Height * 0.2), int(Width * 0.8));
}
