#pragma once

#include "Factory.h"

#include "Screen.h"

#include "UI.h"

#include "SDL_ctx.h"
#include "Stockpile.h"

#include <SDL.h>
#include <SDL_image.h>

#include <array>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

struct Color;
class Country;
class PlayerController;

class MainMenu : public Screen {
public:
    // Constructor, sets default values and creates all needed assets
    MainMenu(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl);

    void ShowCredits();
    void ShowSettings();
    void StartGame();
};

class BackScreen : public Screen {
public:
    inline BackScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : Screen(mw, fp, fpl) {}

    inline void Back() { ChangeScreenFunc(std::make_unique<MainMenu>(*main_window, QuitFunc, ChangeScreenFunc)); }
};

class CreditScreen : public BackScreen {
public:
    CreditScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl);
};

class MenuSettingsScreen : public BackScreen {
public:
    // Constructor, sets default values and creates all needed assets
    MenuSettingsScreen(MainWindow& mw, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl);

    void IncreaseResolution();
    void DecreaseResolution();

    void IncreaseFramerate();
    void DecreaseFramerate();

    void ApplyChanges();

    void onVSyncToggle(bool);
    void onFullscreenToggle(bool);

private:
    unsigned currentResolutionIndex;
    unsigned currentFramerateIndex;

    void UpdateFramerateLabel();
    void UpdateResolutionLabel();
};

class InGameSettingsScreen : public Screen {
public:
};

class GameScreen : public Screen {
public:
    // Constructor, sets default values and creates all needed assets
    GameScreen(MainWindow& mw, const char* tag, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl);

    void Pause();

    void Update(Uint32 elapsedMs) override;
    void RenderBackground() override;
    void Render() override;

    // Handles input events
    void Handle_Input(SDL_Event& ev) override;
    void HandleMouseMovement(SDL_Event& ev);
    void ChangeActiveScreen(std::unique_ptr<Screen> NewScreen, std::string ID);
    void CloseActiveScreen();
    void CloseScreenPreview();

    bool bHasActiveScreen() const { return static_cast<bool>(ActiveScreen); }

private:
    std::unique_ptr<PlayerController> PC;

    bool bIsPaused = false;

    // This is a reference to the pausing screen
    std::unique_ptr<Screen> PM;

    std::unique_ptr<UI> overlay;

    // This is the boolean about whether the screen is zoomable or not
    bool bZoom = true;

    // This is the magnification factor
    double factor = 1.0;

    // This is the current location of the camera along the y axis
    int Cam_Height = 150;

    /*The camera's position along the x axis, as a world x. The map wraps, so
    this is kept in [0, MapWidth) rather than clamped - see WrapX.*/
    int Cam_Width = 2150;

    /*The size of the world in map pixels. The map texture holds exactly one
    copy of it and RenderMap repeats the texture to wrap it horizontally, so
    these are the dimensions of both the texture and the province surface.*/
    static constexpr int MapWidth = 5616;
    static constexpr int MapHeight = 2160;

    // Brings a world x back into [0, MapWidth), wrapping across the date line
    static int WrapX(int x);

    /*How many copies of the world the camera can currently see. Anything drawn
    on the map is drawn once per copy; the renderer clips the ones that miss.*/
    int VisibleCopies() const;

    /*Screen position of a point on the map. ScreenX measures against the copy
    of the world the camera is sitting in; add a multiple of MapWidth for the
    copies either side of it.*/
    int ScreenX(int worldX) const;
    int ScreenY(int worldY) const;

    // Whether a screen y falls on the map at all, rather than past its edge
    bool OnMap(int screenY) const;

    // Draws the map itself, repeated across as much of the screen as it takes
    void RenderMap();

    /*Where the player last clicked, in world coordinates, or nothing while no
    pin is placed. The pin is drawn as one small quad over the map, so placing
    and clearing it costs nothing.*/
    std::optional<SDL_Point> pin;

    // The pin sprite. Its point is the pixel at (PinPointX, PinPointY) within it.
    TextureRef pinTexture;
    static constexpr int PinSize = 32;
    static constexpr int PinPointX = 7;
    static constexpr int PinPointY = 24;

    // Draws the pin, if one is placed, over the map that has just been drawn
    void RenderPin();

    // This is the camera's zooming speed
    double ZoomingSpeed = 0.1;
    bool mousepressed = false;
    int MouseSensitivity = 3;

    std::unique_ptr<Screen> StateViewingScreen;

    std::string ScreenID;
    std::unique_ptr<Screen> ActiveScreen;
};

class PauseMenu : public Screen {
public:
    // Constructor, sets default values and creates all needed assets
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

    void ChangeArrow(int x, int y, int img);

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

    void SendNonAggressionPactRequest();

    void SendAllianceRequest();

    void UpdateRelationValue();
    void UpdateAllianceState();
    void UpdateEmbargoState();
    void UpdateNonAggressionPactState();

    unsigned selectedCountryIndex;
};

class IndustryScreen : public Screen {
public:
    IndustryScreen(MainWindow& mw, Country* Pl);

    void Update(Uint32 elapsedMs) override;

private:
    Country* Player;
};

class TradeScreen : public Screen {
public:
    TradeScreen(MainWindow& mw, Country* Pl);

    void Update(Uint32 elapsedMs) override;

private:
    Country* Player;
};

class EconomyScreen : public Screen {
public:
    EconomyScreen(MainWindow& mw, Country* Pl);

    void Update(Uint32 elapsedMs) override;

    void OnTaxRateChanged();

    void OnHealthcareChanged();

    Country* Player;
};

class OpenFactoryScreen : public Screen {
public:
    OpenFactoryScreen(MainWindow& mw, unsigned id, PlayerController* PC, std::function<void()> fp);

    // Picks the kind the Confirm button will build, and shows what it costs
    void SelectFactory(FactoryType kind);

    void BuildFactory();

    void Close();

    PlayerController* PCref;
    unsigned index;

    // The kind picked so far, once the player has picked one
    std::optional<FactoryType> selected;
};

class StatePreview : public Screen {
public:
    StatePreview(MainWindow& mw, unsigned id, std::string StateName, std::string controller, PlayerController* PC, const std::array<short int, RawGoodCount>& res, int pop, std::string Factories[4],
                 std::function<void()> CloseFunc, std::function<void(std::unique_ptr<Screen>, std::string)> ChangeScreenFunc);

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
