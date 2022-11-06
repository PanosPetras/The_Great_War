#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "ScreenList.h"

class MainWindow {
public:
    //Constructor
    MainWindow();
    //Destructor
    ~MainWindow();

    //The Main loop of the window
    void MainLoop();

    //Handles window input and passes it to the current active screen
    void Keyboard();

    //Sets the guard value of the main loop to 1, terminating the program
    void Quit();

    //Clears the renderer and renders the active screen
    void Render();

    /*Used to transition between screens
    It deletes the already existing one
    And sets the screen that is passed as 
    a parameter as the current active screen*/
    void ChangeScreen(Screen* NewScreen);

public:
    //The game's main window
    SDL_Window* window;

    //The window's renderer
    SDL_Renderer* renderer;

    //The window's cursor
    SDL_Cursor* cursor;

public:
    bool KEYS[322];  // 322 is the number of SDLK_DOWN events

    int quit;

    //Used to store the window's width and Height
    int Width;
    int Height;

    //Stores a pointer to the active screen
    Screen* scr;
};
#endif