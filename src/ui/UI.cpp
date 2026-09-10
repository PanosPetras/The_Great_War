#include "ui/UI.h"

#include "ui/Button.h"
#include "ui/Image.h"
#include "ui/Label.h"
#include "core/MainWindow.h"
#include "game/PlayerController.h"
#include "screens/Screen.h"
#include "screens/ScreenList.h"
#include "ui/ToggleButton.h"

UI::~UI() = default;

UI::UI(MainWindow& mw, const char* tag, PlayerController* PC, std::function<void(std::unique_ptr<Screen>, std::string)> fpl) : main_window(&mw) {
    auto [Width, Height] = mw.GetWindowDimensions();

    // The country management tabs
    std::string flg = std::string("Flags/") + tag;
    flag = std::make_unique<Button>(mw, int(Width * 0.005), int(Height * 0.005), int(Width * 0.05), int(Height * 0.05), flg.c_str(), nullptr, SDLK_Q);

    Buttons[0] = std::make_unique<Button>(mw, int(Width * 0.1), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Technology", nullptr, SDLK_W);
    Buttons[1] = std::make_unique<Button>(mw, int(Width * 0.15), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Economy", [this] { OpenEconomyScreen(); }, SDLK_E);
    Buttons[3] = std::make_unique<Button>(mw, int(Width * 0.2), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Industry", [this] { OpenIndustryScreen(); }, SDLK_R);
    Buttons[2] = std::make_unique<Button>(mw, int(Width * 0.25), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Trade", [this] { OpenTradeScreen(); }, SDLK_T);
    Buttons[4] = std::make_unique<Button>(mw, int(Width * 0.3), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Diplomacy", [this] { OpenDiplomacyScreen(); }, SDLK_Y);
    Buttons[5] = std::make_unique<Button>(mw, int(Width * 0.35), 0, int(Width * 0.032), int(Height * 0.05), "Buttons/UI/Military", nullptr, SDLK_U);

    flagbg = std::make_unique<Image>(mw, "Backgrounds/FlagBg.png", 0, 0, int(Width * 0.06), int(Height * 0.06));

    // The date tab
    Date = std::make_unique<Label>(mw, "1/1/1910", FontSize::Heading, int(Width * 0.81), 0);
    DateButtons[0] = std::make_unique<Button>(mw, int(Width * 0.7), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/Subtract", [this] { DecreaseSpeed(); }, SDLK_KP_MINUS);

    DateButtons[1] = std::make_unique<Button>(mw, int(Width * 0.9), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/Increment", [this] { IncreaseSpeed(); }, SDLK_KP_PLUS);

    PauseButton = std::make_unique<ToggleButton>(mw, int(Width * 0.77), 0, int(Width * 0.022), int(Height * 0.04), "Buttons/UI/pause", "Buttons/UI/play", [this](bool p) { PauseDate(p); }, SDLK_SPACE);

    SpeedBg = std::make_unique<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.7), 0, int(Width * 0.222), int(Height * 0.04));
    SpeedImg = std::make_unique<Image>(mw, "Icons/1-Speed.png", int(Width * 0.735), 0, int(Width * 0.022), int(Height * 0.04));

    // A reference to the player controller
    PCref = PC;

    ChangeScreenFunc = fpl;
}

void UI::Render() {
    // Renders the country menu
    flagbg->Draw();
    flag->Draw();
    for(auto& button : Buttons) {
        button->Draw();
    }

    // Renders the date menu
    SpeedBg->Draw();
    UpdateDateLabel();
    Date->Draw();
    SpeedImg->Draw();
    for(auto& button : DateButtons) {
        button->Draw();
    }
    PauseButton->Draw();
}

void UI::Handle_Input(SDL_Event& ev) {
    // Handles inputs for buttons
    flag->HandleInput(ev);

    for(auto& button : Buttons) {
        button->HandleInput(ev);
    }

    for(auto& button : DateButtons) {
        button->HandleInput(ev);
    }

    PauseButton->HandleInput(ev);
}

void UI::UpdateDateLabel() {
    /*The date changes once per in-game day but this runs every frame, so it is
    the day itself that is checked rather than the string built from it - the
    label would compare an identical string and throw it away almost every
    time, having allocated four of them to get there.*/
    const auto& date = PCref->Date;
    if(date.Day == shownDay && date.Month == shownMonth && date.Year == shownYear) return;

    shownDay = date.Day;
    shownMonth = date.Month;
    shownYear = date.Year;

    Date->ChangeText(std::to_string(date.Day) + "-" + std::to_string(date.Month) + "-" + std::to_string(date.Year));
}

void UI::IncreaseSpeed() {
    PCref->ChangeSpeed(true);
    std::string str = "Icons/" + std::to_string(PCref->Date.Speed) + "-Speed.png";
    SpeedImg->ChangeImage(str);
}

void UI::DecreaseSpeed() {
    PCref->ChangeSpeed(false);
    std::string str = "Icons/" + std::to_string(PCref->Date.Speed) + "-Speed.png";
    SpeedImg->ChangeImage(str.c_str());
}

void UI::PauseDate(bool) {
    PCref->Pause();
}

Country* UI::Player() const {
    return PCref->player;
}

void UI::OpenDiplomacyScreen() {
    ChangeScreenFunc(std::make_unique<DiplomacyScreen>(*main_window, PCref), "DiplomacyScreen");
}

void UI::OpenIndustryScreen() {
    ChangeScreenFunc(std::make_unique<IndustryScreen>(*main_window, Player()), "IndustryScreen");
}

void UI::OpenEconomyScreen() {
    ChangeScreenFunc(std::make_unique<EconomyScreen>(*main_window, Player()), "EconomyScreen");
}

void UI::OpenTradeScreen() {
    ChangeScreenFunc(std::make_unique<TradeScreen>(*main_window, Player()), "TradeScreen");
}
