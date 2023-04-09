#pragma once

#include "Screen.h"

#include <SDL.h>
#include <SDL_image.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

class PlayerController;
class UI;
class Country;

class MainMenu : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    MainMenu(SDL_Renderer* r, std::function<void()> fp , std::function<void(std::unique_ptr<Screen>)> fpl );

    void ShowCredits();
    void ShowSettings();
    void StartGame();
};

class CreditScreen : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    CreditScreen(SDL_Renderer* r, std::function<void()> fp = {}, std::function<void(std::unique_ptr<Screen>)> fpl = {});

    std::function<void(std::unique_ptr<Screen>)> ChangeScreenFunc;
    std::function<void()> QuitFunc;
    void Back();
};

class MenuSettingsScreen : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    MenuSettingsScreen(SDL_Renderer* r, std::function<void()> fp = {}, std::function<void(std::unique_ptr<Screen>)> fpl = {});

    std::function<void(std::unique_ptr<Screen>)> ChangeScreenFunc;
    std::function<void()> QuitFunc;
    void Back();
};

class InGameSettingsScreen : public Screen {
public:
};

class GameScreen : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    GameScreen(SDL_Renderer* r, const char* tag, std::function<void()> fp = {}, std::function<void(std::unique_ptr<Screen>)> fpl = {});
    ~GameScreen();

    PlayerController* PC;

    void Pause();

    void RenderBackground() override;
    void Render() override;

    //Handles input events
    void Handle_Input(SDL_Event* ev) override;

    bool bIsPaused;

    //This is a reference to the pausing screen
    Screen* PM;

    UI* overlay;

    //This is the boolean about whether the screen is zoomable or not
    bool bZoom;

    //This is the magnification factor
    float factor;

    //This is the current location of the camera along the y axis
    int Cam_Height;

    //This is the current location of the camera along the x axis
    int Cam_Width;

    //Stores the dimensions of the image displayed
    int ImgSize[2];

    //This is the camera's zooming speed
    float ZoomingSpeed;
    bool mousepressed;
    int MouseSensitivity;

    Screen *StateViewingScreen;
    bool bHasStatePreview;

    SDL_Texture* assets;

    void HandleMouseMovement(SDL_Event* ev);
    void ChangeActiveScreen(std::unique_ptr<Screen> NewScreen, std::string ID);
    void CloseActiveScreen();
    void CloseScreenPreview();

    bool bHasActiveScreen() const { return static_cast<bool>(ActiveScreen); }
    std::string ScreenID;
    std::unique_ptr<Screen> ActiveScreen;
};

class PauseMenu : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    PauseMenu(SDL_Renderer* r, std::function<void()> fp = {}, std::function<void()> UnpauseF = {}, std::function<void(std::unique_ptr<Screen>)> fpl = {});

    void ReturnToMainMenu();
};

class CountrySelection : public Screen {
public:
    CountrySelection(SDL_Renderer* r, std::function<void()> UnpauseF = {}, std::function<void(std::unique_ptr<Screen>)> fpl = {});

    bool mousepressed;

    const char* tags[8];

    int colors[8][3];

    int CountryIndex;

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
    DiplomacyScreen(SDL_Renderer* r, PlayerController* PC);
    DiplomacyScreen(SDL_Renderer* r, PlayerController* PC, std::string targetTag);
    DiplomacyScreen(SDL_Renderer* r, PlayerController* PC, int index);

    PlayerController* PCref;

private:
    int CreateCountryButtons(PlayerController* PC);
    int CreateCountryButtons(PlayerController* PC, std::string targetTag);

    void SelectCountry(void*);
    void ImproveRelations();
    void WorsenRelations();
    void ImposeEmbargo();

    void SendAllianceRequest();

    void UpdateRelationValue();
    void UpdateAllianceState();
    void UpdateEmbargoState();

    int selectedCountryIndex;
};

class IndustryScreen : public Screen {
public:
    IndustryScreen(SDL_Renderer* r, int Stockpile[30]);
    void UpdateText(int Stockpile[29]);
};

class TradeScreen : public Screen {
public:
    TradeScreen(SDL_Renderer* r, Country* Pl);
    void Update();

private:
    Country* Player;
};

class EconomyScreen : public Screen {
public:
    EconomyScreen(SDL_Renderer* r, Country* Pl);

    void Update();

    void OnTaxRateChanged();

    void OnHealthcareChanged();

    Country* Player;
};

class OpenFactoryScreen : public Screen {
public:
    OpenFactoryScreen(SDL_Renderer* r, int id, PlayerController* PC, std::function<void()> fp);

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
    int index;
    char type;
};

class StatePreview : public Screen {
public:
    StatePreview(SDL_Renderer* r, int id, std::string StateName, std::string controller, PlayerController* PC, int res[8], int pop, std::string Factories[4], std::function<void()> CloseFunc, std::function<void(std::unique_ptr<Screen>, std::string)> ChangeScreenFunc);

    void Render() override;

    void Handle_Input(SDL_Event* ev) override;

    void OpenOFS();

    void DeleteOFS();

private:
    std::string Controller;
    std::function<void(std::unique_ptr<Screen>, std::string)> ChangeScreenFunc2;

    void OpenDiplomacyTab();

    std::unique_ptr<OpenFactoryScreen> OFS;

    int Id;

    PlayerController* PCref;
};
