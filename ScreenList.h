#pragma once

#include "Screen.h"

#include "UI.h"

#include "SDL_ctx.h"

#include <SDL.h>
#include <SDL_image.h>

#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

struct Color;
class Country;
class PlayerController;

class MainMenu : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    MainMenu(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl );

    void ShowCredits();
    void ShowSettings();
    void StartGame();
};

class BackScreen : public Screen {
public:
    inline BackScreen(MainWindow& mw,  std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : Screen(mw, fp, fpl) {}

    inline void Back() {
        ChangeScreenFunc(std::make_unique<MainMenu>(*main_window, QuitFunc, ChangeScreenFunc));
    }
};

class CreditScreen : public BackScreen {
public:
    CreditScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl);
};

class MenuSettingsScreen : public BackScreen {
public:
    //Constructor, sets default values and creates all needed assets
    MenuSettingsScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl);

    void IncreaseResolution();
    void DecreaseResolution();

    void ApplyChanges();
private:
    unsigned currentResolutionIndex;

    void UpdateResolutionLabel();
};

class InGameSettingsScreen : public Screen {
public:
};

class GameScreen : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    GameScreen(MainWindow& mw, const char* tag, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl);

    void Pause();

    void RenderBackground() override;
    void Render() override;

    //Handles input events
    void Handle_Input(SDL_Event& ev) override;
    void HandleMouseMovement(SDL_Event& ev);
    void ChangeActiveScreen(std::unique_ptr<Screen> NewScreen, std::string ID);
    void CloseActiveScreen();
    void CloseScreenPreview();

    bool bHasActiveScreen() const { return static_cast<bool>(ActiveScreen); }

private:
    std::unique_ptr<PlayerController> PC;

    bool bIsPaused = false;

    //This is a reference to the pausing screen
    std::unique_ptr<Screen> PM;

    std::unique_ptr<UI> overlay;

    //This is the boolean about whether the screen is zoomable or not
    bool bZoom = true;

    //This is the magnification factor
    double factor = 1.0;

    //This is the current location of the camera along the y axis
    int Cam_Height = 150;

    //This is the current location of the camera along the x axis
    int Cam_Width = 5384 + 2150;

    //Stores the dimensions of the image displayed
    int ImgSize[2]{16383, 2160};

    //This is the camera's zooming speed
    double ZoomingSpeed = 0.1;
    bool mousepressed = false;
    int MouseSensitivity = 3;

    std::unique_ptr<Screen> StateViewingScreen;

    std::string ScreenID;
    std::unique_ptr<Screen> ActiveScreen;
};

class PauseMenu : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    PauseMenu(MainWindow& mw, std::function<void()> fp = {}, std::function<void()> UnpauseF = {}, std::function<void(std::unique_ptr<Screen>)> fpl = {});

    void ReturnToMainMenu();
};

class CountrySelection : public Screen {
public:
    inline static constexpr unsigned Countries = 8;
    inline static constexpr unsigned no_country_selected = static_cast<unsigned>(-1);

    CountrySelection(MainWindow& mw, std::function<void()> UnpauseF = {}, std::function<void(std::unique_ptr<Screen>)> fpl = {});

    bool mousepressed;

    static const std::array<const char*, Countries> tags;
    static const std::array<Color, Countries> colors;

    unsigned CountryIndex = no_country_selected;

    void SelectGER();
    void SelectENG();
    void SelectFRA();
    void SelectKUK();
    void SelectITA();
    void SelectRUS();
    void SelectOTT();
    void SelectUSA();

    void ChangeArrow(int x,int y, int img);

    void StartGame();

    void Back();
};

class DiplomacyScreen : public Screen {
public:
    DiplomacyScreen(MainWindow& mw, PlayerController* PC, std::string targetTag);
    DiplomacyScreen(MainWindow& mw, PlayerController* PC);
    DiplomacyScreen(MainWindow& mw, PlayerController* PC, unsigned index);

    PlayerController* PCref;

    static inline constexpr unsigned flagsPerLine = 12;

private:
    unsigned CreateCountryButtons(PlayerController* PC);
    unsigned CreateCountryButtons(PlayerController* PC, std::string targetTag);

    void SelectCountry(unsigned index);
    void ImproveRelations();
    void WorsenRelations();
    void ImposeEmbargo();

    void SendAllianceRequest();

    void UpdateRelationValue();
    void UpdateAllianceState();
    void UpdateEmbargoState();

    unsigned selectedCountryIndex;
};

class IndustryScreen : public Screen {
public:
    IndustryScreen(MainWindow& mw, const int(&Stockpile)[30]);
    void UpdateText(const int(&Stockpile)[30]);
};

class TradeScreen : public Screen {
public:
    TradeScreen(MainWindow& mw, Country* Pl);
    void Update();

private:
    Country* Player;
};

class EconomyScreen : public Screen {
public:
    EconomyScreen(MainWindow& mw, Country* Pl);

    void Update();

    void OnTaxRateChanged();

    void OnHealthcareChanged();

    Country* Player;
};

class OpenFactoryScreen : public Screen {
public:
    OpenFactoryScreen(MainWindow& mw, unsigned id, PlayerController* PC, std::function<void()> fp);

    void FactoryTypeLumber();
    void FactoryTypeGlass();
    void FactoryTypeFood();
    void FactoryTypeClothes();
    void FactoryTypeLiquor();
    void FactoryTypeFurniture();
    void FactoryTypeAutomobile();
    void FactoryTypePaper();
    void FactoryTypeTelephone();
    void FactoryTypeRadio();
    void FactoryTypeMachineParts();
    void FactoryTypeElectricGear();
    void FactoryTypeFuel();
    void FactoryTypeCement();
    void FactoryTypeMerchantShip();
    void FactoryTypeSmallArms();
    void FactoryTypeAmmunition();
    void FactoryTypeArtillery();
    void FactoryTypeExplosives();
    void FactoryTypeTank();
    void FactoryTypeAirship();
    void FactoryTypePlane();

    void FactoryType(char t, int cost);

    void BuildFactory();

    void Close();

    //Factory attributes
    int cost;
    PlayerController* PCref;
    unsigned index;
    char type;
};

class StatePreview : public Screen {
public:
    StatePreview(MainWindow& mw, unsigned id, std::string StateName, std::string controller, PlayerController* PC, int res[8], int pop, std::string Factories[4], std::function<void()> CloseFunc, std::function<void(std::unique_ptr<Screen>, std::string)> ChangeScreenFunc);

    void Render() override;

    void Handle_Input(SDL_Event& ev) override;

    void OpenOFS();

    void DeleteOFS();

private:
    std::string Controller;
    std::function<void(std::unique_ptr<Screen>, std::string)> ChangeScreenFunc2;

    void OpenDiplomacyTab();

    std::unique_ptr<OpenFactoryScreen> OFS;

    unsigned Id;

    PlayerController* PCref;
};
