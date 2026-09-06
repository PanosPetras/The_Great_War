#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Color.h"
#include "Coordinate.h"
#include "Country.h"
#include "Diplomacy.h"
#include "State.h"
#include "Stockpile.h"

#include "SDL_ctx.h"

#include <SDL.h>

#include <array>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

class MainWindow;

class PlayerController {
public:
    // Constructor
    PlayerController(MainWindow& mw, const char* tag);

private:
    // Reads every game data file and builds the countries and states from them
    void LoadGameData(const char* tag);

    // Loading data functions
    void InitializeCountries(std::vector<std::string>& names, std::vector<std::string>& tags, const char* tag, const std::vector<Stockpile>& balance);
    void InitializeStates(std::vector<std::string>& owners, std::vector<std::string>& names, std::vector<Coordinate>& coords, const std::vector<int>& populations, std::vector<Color>& colors);

    /*Decode the map assets. These are safe to run off the main thread because
    they only touch SDL_Surfaces, which are plain CPU memory. They must never
    touch the renderer - see UploadAssets.*/
    void LoadMap();
    void LoadUtilityAssets();

    /*Hand the decoded surfaces to the GPU. SDL_Renderer is main-thread only
    (SDL_render.h: "These functions must be called from the main thread"), so
    this runs after the loading threads have been joined.*/
    void UploadAssets();

    // Advances the calendar by a single day, rolling the month and year over
    void AdvanceOneDay();

    // How long a single in-game day lasts, in real milliseconds, at the current speed
    Uint32 MillisecondsPerDay() const;

    /*Never simulate more than this many days in one frame. Without the cap a
    long stall - loading a screen, dragging the window - would be paid back as
    a burst of ticks that stutters the frame it lands on.*/
    static constexpr Uint32 MaxCatchUpDays = 4;

    // Real time that has elapsed but not yet been converted into game days
    Uint32 dayAccumulator = 0;

    // Decoded but not yet uploaded map assets. Released by UploadAssets.
    SDL_Surface_ctx mapCanvas;
    SDL_Surface_ctx overlayCanvas;

public:
    MainWindow* main_window;

    /*The country the player is in control of. Non-owning - the Country itself
    lives in Countries, which outlives every screen that looks at it.*/
    Country* player = nullptr;

    // The in-game date
    struct {
        int Year;
        int Month;
        int Day;
        int Speed;
        int MonthDays[12];
    } Date;

    bool bIsPaused = true;

    // This is the representing the pass of a single day
    void Tick();

    /*Advances the simulation by however much real time has passed since the
    previous frame. Called from the main loop, so all game state is only ever
    touched by the thread that renders it.*/
    void Update(Uint32 elapsedMs);

    // Pauses or resumes the flow of time
    void Pause();

    // Changes the game speed
    void ChangeSpeed(bool change);

    Market WorldMarket;

    // Reference to every single state on the map
    std::vector<State> StatesArr;
    std::unordered_map<std::string, State*> StatesMap;

    // Owns every country in the game. Country pointers elsewhere point in here.
    std::vector<std::unique_ptr<Country>> Countries;

    // The state of the diplomatic relations between every country
    Diplomacy diplo;

    // Some SDL assets needed
    SDL_Texture_ctx txt;
    SDL_Texture_ctx overlay;
    SDL_Surface_ctx map;
    SDL_Surface_ctx provinces;
};
#endif
