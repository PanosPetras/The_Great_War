#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Color.h"
#include "Coordinate.h"
#include "Country.h"
#include "Diplomacy.h"
#include "State.h"
#include "SDL_ctx.h"

#include <SDL.h>

#include <array>
#include <atomic>
#include <cstdint>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

class PlayerController {
public:
    //Constructor
    PlayerController(SDL_Renderer_ctx& r, const char* tag);

    ~PlayerController();

private:
    //Loading data functions
    void InitializeCountries(std::vector<std::string>& names, std::vector<std::string>& tags, const char* tag, std::vector<int>& balance);
    void InitializeStates(std::vector<std::string>& owners, std::vector<std::string>& names, std::vector<Coordinate>& coords, const std::vector<int>& populations, std::vector<Color>& colors);

    void LoadMap();
    void LoadUtilityAssets();
	
public:

    //Some info about the player
    std::string player_tag;
    int player_index;

    //The in-game date
    struct {
        int Year;
        int Month;
        int Day;
        int Speed;
        int MonthDays[12];
    } Date;

    std::atomic<bool> bIsPaused{true};

    //This is the representing the pass of a single day
    void Tick();

    //Advances the date by one day
    void AdvanceDate();

    //Pauses the flow of time
    void Pause();

    //Changes the game speed
    void ChangeSpeed(bool change);

    Market WorldMarket;

    //Reference to every single state on the map
    std::vector<State> StatesArr;
    std::unordered_map<std::string, State*> StatesMap;

    //Reference to every country
    std::vector<std::unique_ptr<Country>> CountriesArr;

    //The state of the diplomatic relations between every country
    Diplomacy diplo;

    //Some SDL assets needed
    RendererRef RendererReference;
    SDL_Texture_ctx txt;
    SDL_Texture_ctx overlay;
    SDL_Surface_ctx map;
    SDL_Surface_ctx provinces;

    //Used to run the time-functionality of the game
    std::jthread thread;
};
#endif
