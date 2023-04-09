#include "MainWindow.h"
#include "WindowInfo.h"
#include "ScreenList.h"

#include <iostream>

void MainWindow::MainLoop() {
    while (!quit) {            
        this->Render();
        this->Keyboard();
    }
}

MainWindow::MainWindow():
    sdl_init_ctx{}, window{}, renderer(window), ttf_init_ctx{}, img_init_ctx{}, mix_ctx{}, cursor{}
{
    //Get dimensions of the screen
    SDL_GetWindowSize(window, &WindowInfo::width, &WindowInfo::height);

    //Initializing the input array
    for (int i = 0; i < 322; i++) {
        KEYS[i] = false;
    }

    //Initializing the quit variable, this ends the main loop
    quit = 0;

    //Creating a pointer to the active screen
    scr = std::make_unique<MainMenu>(renderer, [this]{ Quit(); }, [this](std::unique_ptr<Screen> newScreen){ ChangeScreen(std::move(newScreen)); });
}

void MainWindow::Render() {
    //Remove everything from the screen
    SDL_RenderClear(renderer);
    
    //Create all textures and add them to the renderer
    scr->Render();

    //Render the results
    SDL_RenderPresent(renderer);
}

void MainWindow::ChangeScreen(std::unique_ptr<Screen> NewScreen){
    scr = std::move(NewScreen);
}

void MainWindow::Keyboard() {
    // message processing loop
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // check for messages
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                KEYS[event.key.keysym.sym] = true;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                KEYS[event.key.keysym.sym] = false;
                break;
            default:
                break;
            }
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                Quit();
            }
            break;
        // exit if the window is closed
        case SDL_QUIT:
            KEYS[SDLK_ESCAPE] = true;
            break;
        default:
            break;
        }
        // pass event to screen
        if(scr) {
            std::cerr << "MainWindow::Keyboard " << static_cast<void*>(scr.get()) << std::endl;
            scr->Handle_Input(event);
        } else {
            std::cerr << "invalid scr" << std::endl;
        }
    } // end of message processing
}

void MainWindow::Quit() {
    //Change the flag variable to 1 so that the main loop stops running
    quit = 1;
}
