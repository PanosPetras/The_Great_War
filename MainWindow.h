#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Screen.h"

#include "SDL_ctx.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <functional>
#include <utility>
#include <vector>
#include <array>

class MainWindow {
public:
    // Singleton instance interface
    static MainWindow& Instance();

    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

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
    void ChangeScreen(std::unique_ptr<Screen> NewScreen);

    // Use AddEvent to add deferred events, like button clicks that may
    // destroy the component while it's processing events.
    template<class Event>
    void AddEvent(Event&& ev) {
        event_queue.emplace_back(std::forward<Event>(ev));
    }

private:
    MainWindow(); //Constructor for Instance()

    // Note that the order of the SDL context instances is important
    SDL_Init_ctx sdl_init_ctx;
    SDL_Window_ctx window; //The game's main window
    SDL_Renderer_ctx renderer; //The window's renderer
    TTF_Init_ctx ttf_init_ctx;
    IMG_Init_ctx img_init_ctx;
    MIX_ctx mix_ctx;
    SDL_Cursor_ctx cursor; //The window's cursor

    std::array<bool, 322> KEYS;  // 322 is the number of SDLK_DOWN events

    bool quit = false;

    //Stores a pointer to the active screen
    std::unique_ptr<Screen> scr;

    std::vector<std::function<void()>> event_queue; // deferred events

    //Used to store the window's width and Height:
    static inline SDL_Point windim{};

    friend int GetWindowWidth();
    friend int GetWindowHeight();
    friend const SDL_Point& GetWindowDimensions();
};

// For convenience:
inline int GetWindowWidth() { return MainWindow::windim.x; }
inline int GetWindowHeight() { return MainWindow::windim.y; }
inline const SDL_Point& GetWindowDimensions() { return MainWindow::windim; }

#endif
