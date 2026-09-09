#pragma once

#include "game/Factory.h"

#include "screens/Screen.h"

#include "ui/UI.h"

#include "core/SDL_ctx.h"
#include "game/Stockpile.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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
    void HandleKeyboardPanning(Uint32 elapsedMs);
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

    /*How far down the camera can sit before the bottom of the map would lift
    off the bottom of the screen, and the check that keeps it there.*/
    int MaxCamHeight() const;
    void ClampCamHeight();

    /*Moves the camera by a distance in map pixels. Every way of moving it -
    dragging, scrolling, the arrow keys, zooming - goes through here.*/
    void PanCamera(double dx, double dy);

    /*Leftovers from PanCamera. The camera sits on whole map pixels, but a
    trackpad moves it by fractions of one at a time, and truncating each of
    those away would leave fine scrolling moving nothing at all.*/
    double PanRemainderX = 0;
    double PanRemainderY = 0;

    /*Changes the magnification, keeping whatever is in the middle of the
    screen in the middle of the screen. A step is one notch of a mouse wheel,
    but it is fractional: a trackpad and a pinch both arrive in slices.*/
    void Zoom(double steps);

    /*A scroll either pans or zooms depending on where it came from, since a
    trackpad has no other way to reach the map and a wheel has always zoomed.*/
    void HandleScroll(const SDL_MouseWheelEvent& wheel);
    bool IsTrackpadScroll(const SDL_MouseWheelEvent& wheel);

    /*Zooming by pinch. SDL3 has no gesture API, so the fingers that make up a
    pinch are followed here - see the definition.*/
    void HandlePinch(const SDL_Event& ev);

    // The two fingers a pinch is measured between, and how far apart they were
    struct Finger {
        SDL_FingerID id;
        float x;
        float y;
    };
    std::array<Finger, 2> fingers{};
    int fingerCount = 0;
    float PinchDistance = 0;

    /*When a scroll last looked like it came from a trackpad, and how long a
    device is taken to still be one - see IsTrackpadScroll. SDL3 stamps events
    in nanoseconds rather than the milliseconds SDL2 used, so both are kept in
    nanoseconds to compare against an event's timestamp directly.*/
    Uint64 LastTrackpadScrollNs = 0;
    static constexpr Uint64 TrackpadScrollMemoryNs = 600 * SDL_NS_PER_MS;

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

    // How far one step of scrolling pans the map, in screen pixels
    double ScrollingSpeed = 40;

    // How far the arrow keys pan the map, in screen pixels per millisecond
    double KeyboardSpeed = 0.8;

    // How many steps of zoom a pinch is worth, per unit of the distance closed
    double PinchingSpeed = 60;

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
