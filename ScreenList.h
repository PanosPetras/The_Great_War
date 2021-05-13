#include <SDL.h>
#include <SDL_Image.h>
#include <functional>
#include <string>
#include "PlayerController.h"
#include "Screen.h"

#ifndef UI_H
#define UI_H

class UI {
public:
    UI(SDL_Renderer* r, int Width, int Height, const char* tag, PlayerController* PC, std::function<void(Screen*)> fpl = NULL);
    //Destructor
    ~UI();

    bool bDateUpdated;

    //This is a reference to the main window's renderer
    SDL_Renderer* renderer;

    //Stores the main window's dimensions
    int WindowSize[2];

    //Renders all of the screen's components
    void Render();

    //Handles input events
    void Handle_Input(SDL_Event* ev);

    void IncreaseSpeed();
    void DecreaseSpeed();
    void PauseDate();

    Label* Date;

    Button* DateButtons[3];

    Image* SpeedBg;
    Image* SpeedImg;

    Button* flag;

    Button* Buttons[5];

    Image* flagbg;

    PlayerController* PCref;

    std::function<void(Screen*)> ChangeScreenFunc;

    void OpenIndustryScreen();
};

#endif

#ifndef MainMenu_H
#define MainMenu_H

class MainMenu : public Screen{
public:
    //Constructor, sets default values and creates all needed assets
    MainMenu(SDL_Renderer* r, int Width, int Height, std::function<void()> fp = NULL, std::function<void(Screen*)> fpl = NULL);
    void ShowCredits();
    void StartGame();
};

#endif

#ifndef CreditScreen_H
#define CreditScreen_H

class CreditScreen : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    CreditScreen(SDL_Renderer* r, int Width, int Height, std::function<void()> fp = NULL, std::function<void(Screen*)> fpl = NULL);

    std::function<void(Screen*)> ChangeScreenFunc;
    std::function<void()> QuitFunc;
    void Back();
};

#endif

#ifndef GameScreen_H
#define GameScreen_H

class GameScreen : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    GameScreen(SDL_Renderer* r, int Width, int Height, const char* tag, std::function<void()> fp = NULL, std::function<void(Screen*)> fpl = NULL);
    ~GameScreen() override;

    PlayerController* PC;

    void Pause();

    void SetBackground() override;
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

    /*Stores the state in which the camera is along the y axis:
    0: Not moving
    1: Moving to the left
    2: Moving to the right*/
    int YState;

    /*Stores the state in which the camera is along the x axis:
    0: Not moving
    1: Moving to the left
    2: Moving to the right*/
    int XState;

    //This is the camera's zooming speed
    float ZoomingSpeed;
    bool mousepressed;
    bool once;
    int MouseSensitivity;

    Screen *StateViewingScreen;
    bool bHasStatePreview;

    SDL_Texture* assets;

    void HandleMouseMovement(SDL_Event* ev);
    void ChangeActiveScreen(Screen* NewScreen);
    void CloseActiveScreen();
    void CloseScreenPreview();

    bool bHasActiveScreen;
    Screen* ActiveScreen;
};

#endif

#ifndef PauseMenu_H
#define PauseMenu_H

class PauseMenu : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    PauseMenu(SDL_Renderer* r, int Width, int Height, std::function<void()> fp = NULL, std::function<void()> UnpauseF = NULL, std::function<void(Screen*)> fpl = NULL);

    void ReturnToMainMenu();
};

#endif

#ifndef CountrySelection_H
#define CountrySelection_H

class CountrySelection : public Screen {
public:
    CountrySelection(SDL_Renderer* r, int Width, int Height, std::function<void()> UnpauseF = NULL, std::function<void(Screen*)> fpl = NULL);

    bool mousepressed;

    SDL_Texture* texture;

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

#endif

#ifndef IndustryScreen_H
#define IndustryScreen_H

class IndustryScreen : public Screen {
public:
    IndustryScreen(SDL_Renderer* r, int Width, int Height, int Stockpile[30], std::function<void()> fp = NULL, std::function<void(Screen*)> fpl = NULL);
    void UpdateText(int Stockpile[29]);
};

#endif

#ifndef IndustryScreen1_H
#define IndustryScreen1_H

class IndustryScreen1 : public Screen {
public:
    IndustryScreen1(SDL_Renderer* r, int Width, int Height, int Stockpile[30], std::function<void()> fp = NULL, std::function<void(Screen*)> fpl = NULL);
};

#endif

#ifndef StatePreview_H
#define StatePreview_H

class StatePreview : public Screen {
public:
    StatePreview(SDL_Renderer* r, int Width, int Height, std::string StateName, std::string Controller, int res[8], int pop, std::string Factories[4], std::function<void()> CloseFunc);
};

#endif 