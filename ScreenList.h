#include <SDL.h>
#include <SDL_Image.h>
#include <functional>
#include <string>
#include <vector>
#include "PlayerController.h"
#include "Screen.h"

class UI {
public:
    //Constructor
    UI(SDL_Renderer* r, int Width, int Height, const char* tag, PlayerController* PC, std::function<void(Screen*, std::string)> fpl = NULL);

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
    Button* DateButtons[3];
    Button* flag;

    PlayerController* PCref;

    std::function<void(Screen*, std::string)> ChangeScreenFunc;

    //Functions

    //Date related
    void IncreaseSpeed();
    void DecreaseSpeed();
    void PauseDate();

    void OpenIndustryScreen();
    void OpenEconomyScreen();
    void OpenTradeScreen();
};

class MainMenu : public Screen{
public:
    //Constructor, sets default values and creates all needed assets
    MainMenu(SDL_Renderer* r, int Width, int Height, std::function<void()> fp = NULL, std::function<void(Screen*)> fpl = NULL);
    void ShowCredits();
    void StartGame();
};

class CreditScreen : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    CreditScreen(SDL_Renderer* r, int Width, int Height, std::function<void()> fp = NULL, std::function<void(Screen*)> fpl = NULL);

    std::function<void(Screen*)> ChangeScreenFunc;
    std::function<void()> QuitFunc;
    void Back();
};

class GameScreen : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    GameScreen(SDL_Renderer* r, int Width, int Height, const char* tag, std::function<void()> fp = NULL, std::function<void(Screen*)> fpl = NULL);
    ~GameScreen() override;

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
    void ChangeActiveScreen(Screen* NewScreen, std::string ID);
    void CloseActiveScreen();
    void CloseScreenPreview();

    bool bHasActiveScreen;
    std::string ScreenID;
    Screen* ActiveScreen;
};

class PauseMenu : public Screen {
public:
    //Constructor, sets default values and creates all needed assets
    PauseMenu(SDL_Renderer* r, int Width, int Height, std::function<void()> fp = NULL, std::function<void()> UnpauseF = NULL, std::function<void(Screen*)> fpl = NULL);

    void ReturnToMainMenu();
};

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

class IndustryScreen : public Screen {
public:
    IndustryScreen(SDL_Renderer* r, int Width, int Height, int Stockpile[30]);
    void UpdateText(int Stockpile[29]);
};

class TradeScreen : public Screen {
public:
    TradeScreen(SDL_Renderer* r, int Width, int Height, Country* Pl);
    void Update();

    Country* Player;
};

class EconomyScreen : public Screen {
public:
    EconomyScreen(SDL_Renderer* r, int Width, int Height, Country* Pl);

    void Update();

    Country* Player;
};

class OpenFactoryScreen : public Screen {
public:
    OpenFactoryScreen(SDL_Renderer* r, int Width, int Height, int id, PlayerController* PC, std::function<void()> fp);

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
    OpenFactoryScreen* OFS = nullptr;

    int Id;

    StatePreview(SDL_Renderer* r, int Width, int Height, int id, std::string StateName, std::string Controller, PlayerController* PC, int res[8], int pop, std::string Factories[4], std::function<void()> CloseFunc);

    ~StatePreview();

    PlayerController* PCref;

    void Render() override;

    void Handle_Input(SDL_Event* ev) override;

    void OpenOFS();

    void DeleteOFS();
};