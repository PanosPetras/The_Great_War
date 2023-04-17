#include "MainWindow.h"

#include "Button.h"

#include "ScreenList.h"

#include <iostream>

MainWindow& MainWindow::Instance() {
    static MainWindow inst;
    return inst;
}

MainWindow::MainWindow():
    //sdl_init_ctx{}, window{}, renderer(window), ttf_init_ctx{}, img_init_ctx{}, mix_ctx{}, cursor{}
    renderer(window)
{
    // Ugly fix - Use static variables to avoid deadlock when MainMenu needs the dimensions.
    // An alternative would be to supply the dimensions to screens being constructed.
    windim = window.GetWindowDimensions();

    //Initializing the input array
    for (bool& i : KEYS) {
        i = false;
    }

    //Creating a pointer to the active screen
    scr = std::make_unique<MainMenu>(*this, [this]{ Quit(); }, [this](std::unique_ptr<Screen> newScreen){ ChangeScreen(std::move(newScreen)); });
}

void MainWindow::MainLoop() {
    while (!quit) {            
        Render();
        Keyboard();
    }
}

void MainWindow::Render() {
    //Remove everything from the screen
    SDL_RenderClear(renderer);
    
    //Create all textures and add them to the renderer
    scr->Render();

    //Render the results
    SDL_RenderPresent(renderer);
}

SDL_Texture_ctx& MainWindow::IMG_Load(const std::string& filename) {
    if(auto it = file_textures.find(filename); it != file_textures.end()) {
        return it->second[0];
    } else {
        std::vector<SDL_Texture_ctx> vec;
        vec.emplace_back(SDL_Texture_ctx::IMG_Load(renderer, filename));
        auto [newit, inserted] = file_textures.emplace(filename, std::move(vec));
        return newit->second[0];
    }
}

MainWindow::operator SDL_Renderer_ctx& () {
    return renderer;
}

MainWindow::operator SDL_Renderer* () {
    return renderer;
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
        scr->Handle_Input(event);

        // execute events added by screen and its children
        if(not quit) {
            for(auto&& ev : event_queue) {
                std::cerr << "MainWindow::Keyboard\tcalling deferred event" << std::endl;
                ev();
            }
            event_queue.clear();
        }
    } // end of message processing
}

void MainWindow::Quit() {
    //Change the flag variable to 1 so that the main loop stops running
    quit = true;
}

ChunkRef MainWindow::Mix_LoadWAV(const std::string& filename) {
    if(auto it = file_chunks.find(filename); it != file_chunks.end()) {
        return it->second;
    }
    auto [newit, inserted] = file_chunks.emplace(filename, filename);
    return newit->second;
}

int MainWindow::Width() const { return windim.x; }
int MainWindow::Height() const { return windim.y; }
const SDL_Point& MainWindow::GetWindowDimensions() const { return windim; }
