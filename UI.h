#ifndef UI_H
#define UI_H

#pragma once

#include "ToggleButton.h"

#include <SDL.h>
#include <array>
#include <functional>
#include <memory>
#include <string>

class Screen;
class Image;
class Label;
class Button;
class ToggleButton;
class PlayerController;

class UI {
public:
    //Constructor
    UI(SDL_Renderer* r, const char* tag, PlayerController* PC, std::function<void(Screen*, std::string)> fpl = nullptr);
    ~UI() = default;

    //This is a reference to the main window's renderer
    SDL_Renderer* renderer;

    //Stores the main window's dimensions
    int WindowSize[2];

    //Renders all of the screen's components
    void Render();

    //Handles input events
    void Handle_Input(SDL_Event* ev);

    std::unique_ptr<Label> Date;
    std::unique_ptr<Label> Balance;

    std::unique_ptr<Image> SpeedBg;
    std::unique_ptr<Image> SpeedImg;
    std::unique_ptr<Image> flagbg;

    std::array<std::unique_ptr<Button>, 6> Buttons;
    std::array<std::unique_ptr<Button>, 2> DateButtons;
    std::unique_ptr<ToggleButton> PauseButton;
    std::unique_ptr<Button> flag;

    PlayerController* PCref;

    std::function<void(Screen*, std::string)> ChangeScreenFunc;

    //Functions

    //Date related
    void IncreaseSpeed();
    void DecreaseSpeed();
    void PauseDate(bool);

    void OpenDiplomacyScreen();
    void OpenIndustryScreen();
    void OpenEconomyScreen();
    void OpenTradeScreen();
};

#endif
