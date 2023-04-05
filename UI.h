#ifndef UI_H
#define UI_H

#pragma once

#include <SDL.h>
#include <functional>

class Screen;
class Image;
class Label;
class Button;
class ToggleButton;
class PlayerController;

class UI {
public:
    //Constructor
    UI(SDL_Renderer* r, const char* tag, PlayerController* PC, std::function<void(Screen*, std::string)> fpl = NULL);

    //Destructor
    ~UI();

    //This is a reference to the main window's renderer
    SDL_Renderer* renderer;

    //Stores the main window's dimensions
    int WindowSize[2];

    //Renders all of the screen's components
    void Render();

    //Handles input events
    void Handle_Input(SDL_Event* ev);

    Label* Date;
    Label* Balance;

    Image* SpeedBg;
    Image* SpeedImg;
    Image* flagbg;

    Button* Buttons[6];
    Button* DateButtons[2];
    ToggleButton* PauseButton;
    Button* flag;

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
