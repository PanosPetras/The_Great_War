#include "ScreenList.h"

#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "PlayerController.h"
#include "UI.h"

const std::array<const char*, CountrySelection::Countries> CountrySelection::tags {
    "ger",
    "fra",
    "eng",
    "kuk",
    "ita",
    "rus",
    "ott",
    "usa"
};

const std::array<Color, CountrySelection::Countries> CountrySelection::colors{{
    {42, 42, 42},
    {18, 30, 152},
    {128, 0, 32},
    {255, 255, 255},
    {38, 160, 64},
    {64, 160, 96},
    {160, 144, 96},
    {0, 96, 128}
}};

CountrySelection::CountrySelection(MainWindow& mw, std::function<void()> UnpauseF, std::function<void(std::unique_ptr<Screen>)> fpl) : Screen(mw, UnpauseF, fpl) {
	SetupBg("Backgrounds/CountrySelection.png");
        auto [Width, Height] = mw.GetWindowDimensions();

	AddDrawable<Button>(mw, int(Width * 0.85), int(Height * 0.85), int(Width * 0.09), int(Height * 0.06), "Confirm", 32, [this]{StartGame();}, SDLK_KP_ENTER);
	AddDrawable<Button>(mw, int(Width * 0.1), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, [this]{ Back();}, SDLK_ESCAPE);

	AddDrawable<Button>(mw, int(Width * 0.3), int(Height * 0.2), int(Width * 0.06), int(Height * 0.06), "Flags/ger", [this]{ SelectGER();});
	AddDrawable<Button>(mw, int(Width * 0.3), int(Height * 0.3), int(Width * 0.06), int(Height * 0.06), "Flags/fra", [this]{ SelectFRA();});
	AddDrawable<Button>(mw, int(Width * 0.3), int(Height * 0.4), int(Width * 0.06), int(Height * 0.06), "Flags/eng", [this]{ SelectENG();});
	AddDrawable<Button>(mw, int(Width * 0.3), int(Height * 0.5), int(Width * 0.06), int(Height * 0.06), "Flags/kuk", [this]{ SelectKUK();});
	AddDrawable<Button>(mw, int(Width * 0.635), int(Height * 0.3), int(Width * 0.06), int(Height * 0.06), "Flags/ita", [this]{ SelectITA();});
	AddDrawable<Button>(mw, int(Width * 0.635), int(Height * 0.4), int(Width * 0.06), int(Height * 0.06), "Flags/rus", [this]{ SelectRUS();});
	AddDrawable<Button>(mw, int(Width * 0.635), int(Height * 0.5), int(Width * 0.06), int(Height * 0.06), "Flags/ott", [this]{ SelectOTT();});
	AddDrawable<Button>(mw, int(Width * 0.635), int(Height * 0.2), int(Width * 0.06), int(Height * 0.06), "Flags/usa", [this]{ SelectUSA();});

	AddLabel<Label>(mw, "The World in 1910", int(Height * 0.03), int(Width * 0.43), int(Height * 0.07));
	AddLabel<Label>(mw, "Choose your nation", int(Height * 0.03), int(Width * 0.427), int(Height * 0.13));
	AddLabel<Label>(mw, "German Empire", int(Height * 0.023), int(Width * 0.37), int(Height * 0.21));
	AddLabel<Label>(mw, "French Republic", int(Height * 0.023), int(Width * 0.37), int(Height * 0.31));
	AddLabel<Label>(mw, "British Empire", int(Height * 0.023), int(Width * 0.37), int(Height * 0.41));
	AddLabel<Label>(mw, "Austria-Hungary", int(Height * 0.023), int(Width * 0.37), int(Height * 0.51));
	AddLabel<Label>(mw, "United States", int(Height * 0.023), int(Width * 0.55), int(Height * 0.21));
	AddLabel<Label>(mw, "Kingdom of Italy", int(Height * 0.023), int(Width * 0.53), int(Height * 0.31));
	AddLabel<Label>(mw, "Russian Empire", int(Height * 0.023), int(Width * 0.538), int(Height * 0.41));
	AddLabel<Label>(mw, "Ottoman Empire", int(Height * 0.023), int(Width * 0.53), int(Height * 0.51));

	AddImage<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.428), int(Height * 0.07), int(Width * 0.14), int(Height * 0.04));
	AddImage<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.425), int(Height * 0.13), int(Width * 0.146), int(Height * 0.04));
	AddImage<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.296), int(Height * 0.195), int(Width * 0.069), int(Height * 0.072));
	AddImage<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.296), int(Height * 0.295), int(Width * 0.069), int(Height * 0.072));
	AddImage<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.296), int(Height * 0.395), int(Width * 0.069), int(Height * 0.072));
	AddImage<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.296), int(Height * 0.495), int(Width * 0.069), int(Height * 0.072));
	AddImage<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.631), int(Height * 0.195), int(Width * 0.069), int(Height * 0.072));
	AddImage<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.631), int(Height * 0.295), int(Width * 0.069), int(Height * 0.072));
	AddImage<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.631), int(Height * 0.395), int(Width * 0.069), int(Height * 0.072));
	AddImage<Image>(mw, "Backgrounds/FlagBg.png", int(Width * 0.631), int(Height * 0.495), int(Width * 0.069), int(Height * 0.072));
	AddImage<Image>(mw, "Icons/right.png", -Width, -Height, 64 * Width / 1920, 64 * Height / 1080);

	mousepressed = false;

	CountryIndex = -1;
}

void CountrySelection::Back(){
    ChangeScreenFunc(std::make_unique<MainMenu>(*main_window, QuitFunc, ChangeScreenFunc));
}

void CountrySelection::SelectGER() {
	CountryIndex = 0;
	ChangeArrow(int(main_window->Width() * 0.262), int(main_window->Height() * 0.202), 1);
}

void CountrySelection::SelectENG() {
	CountryIndex = 2;
	ChangeArrow(int(main_window->Width() * 0.262), int(main_window->Height() * 0.402), 1);
}

void CountrySelection::SelectFRA() {
	CountryIndex = 1;
	ChangeArrow(int(main_window->Width() * 0.262), int(main_window->Height() * 0.302), 1);
}

void CountrySelection::SelectKUK() {
	CountryIndex = 3;
	ChangeArrow(int(main_window->Width() * 0.262), int(main_window->Height() * 0.502), 1);
}

void CountrySelection::SelectITA() {
	CountryIndex = 4;
	ChangeArrow(int(main_window->Width() * 0.7), int(main_window->Height() * 0.302), 2);
}

void CountrySelection::SelectRUS() {
	CountryIndex = 5;
	ChangeArrow(int(main_window->Width() * 0.7), int(main_window->Height() * 0.402), 2);
}

void CountrySelection::SelectOTT() {
	CountryIndex = 6;
	ChangeArrow(int(main_window->Width() * 0.7), int(main_window->Height() * 0.502), 2);
}

void CountrySelection::SelectUSA() {
	CountryIndex = 7;
	ChangeArrow(int(main_window->Width() * 0.7), int(main_window->Height() * 0.202), 2);
}

void CountrySelection::ChangeArrow(int x, int y, int img) {
	ImageArr[10]->ChangePosition(x, y, int(64 * main_window->Width() / 1920), int(64 * main_window->Height() / 1080));
	if (img == 1) {
		ImageArr[10]->ChangeImage("Icons/right.png");
	} else if (img == 2) {
		ImageArr[10]->ChangeImage("Icons/left.png");
	}
}

void CountrySelection::StartGame() {
	if (CountryIndex != -1) {
		ChangeScreenFunc(std::make_unique<GameScreen>(*main_window, tags[CountryIndex], QuitFunc, ChangeScreenFunc));
	}
}
