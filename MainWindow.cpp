#include "MainWindow.h"

using namespace std;

void MainWindow::MainLoop() {
    while (!quit) {            
        this->Render();
        this->Keyboard();
    }
}

MainWindow::MainWindow() {
    // Initialize SDL2
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "The Great War",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        1920,                               // width, in pixels
        1080,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    //Set the window to fullscreen mode
    //SDL_SetWindowFullscreen(window, 1); 

    //Initializing the window renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    //Initializing the rendring libraries
    TTF_Init();
    IMG_Init(0);

    //Initializing the audio playing library
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_AllocateChannels(2);
    //Mix_Init(0);

    //Get dimensions of the screen
    int* w = &Width;
    int* h = &Height;
    SDL_GetWindowSize(window, w, h);

    //Initializing the input array
    for (int i = 0; i < 322; i++) {
        KEYS[i] = false;
    }

    //Initializing the quit variable, this ends the main loop
    quit = 0;

    //Creates the cursor
    SDL_Surface* surface = IMG_Load("Icons/mouse.png");
    cursor = SDL_CreateColorCursor(surface, 1, 1);
    SDL_SetCursor(cursor);
    SDL_FreeSurface(surface);

    //Creating a pointer to the active screen
    auto fp = std::bind(&MainWindow::Quit, this);
    auto fpl = std::bind(&MainWindow::ChangeScreen, this, std::placeholders::_1);
    scr = new MainMenu(renderer, Width, Height, fp, fpl);
}

MainWindow::~MainWindow() {
    //Delete the active screen
    delete scr;

    //Free the memory allocated to the cursor
    SDL_FreeCursor(cursor);

    //Free all the allocated memory
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_DestroyRenderer(renderer);

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
}

void MainWindow::Render() {
    //Remove everything from the screen
    SDL_RenderClear(renderer);
    
    //Create all textures and add them to the renderer
    scr->Render();

    //Render the results
    SDL_RenderPresent(renderer);
}

void MainWindow::ChangeScreen(Screen* NewScreen){
    //Delete the active screen
    delete scr;

    //Save the new screen
    scr = NewScreen;
}

void MainWindow::Keyboard() {
    // message processing loop
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // check for messages
        switch (event.type) {
            // exit if the window is closed
        case SDL_QUIT:
            KEYS[SDLK_ESCAPE] = true;
            break;
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
        default:
            break;
        }
        // pass event to screen
        scr->Handle_Input(&event);
    } // end of message processing
}

void MainWindow::Quit() {
    //Change the flag variable to 1 so that the main loop stops running
    quit = 1;
}