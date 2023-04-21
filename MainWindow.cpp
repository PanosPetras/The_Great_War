#include "MainWindow.h"

#include "Button.h"

#include "ScreenList.h"

#include <iostream>

MainWindow& MainWindow::Instance() {
    static MainWindow inst;
    return inst;
}

MainWindow::MainWindow() :
    // sdl_init_ctx{}, window{}, renderer(window), ttf_init_ctx{}, img_init_ctx{}, mix_ctx{}, cursor{}
    renderer(window), windim(window.GetWindowDimensions()), scr(std::make_unique<MainMenu>(
                                                                *this, [this] { Quit(); }, [this](std::unique_ptr<Screen> newScreen) { ChangeScreen(std::move(newScreen)); })) {
    vsync = true;
    fullscreen = true;
    framerateCap = 60;
}

void MainWindow::MainLoop() {
    unsigned int a, b = SDL_GetTicks();
    double delta = 0;

    while(!quit) {
        a = SDL_GetTicks();

        if(a - b >= 1000.0 / framerateCap || vsync) {
            b = a;

            Render();
            Keyboard();
        }
    }
}

void MainWindow::Render() {
    // Remove everything from the screen
    SDL_RenderClear(renderer);

    // Create all textures and add them to the renderer
    scr->Render();

    // Render the results
    SDL_RenderPresent(renderer);
}

SDL_Texture_ctx& MainWindow::IMG_Load(const std::string& filename) {
    if(auto it = file_textures.find(filename); it != file_textures.end()) {
        // NOTE: If a Button has already loaded this surface, three textures will have been
        // placed in the std::vector<SDL_Texture_ctx>. idle, hoovered and inactive.
        return it->second[0];
    } else {
        std::vector<SDL_Texture_ctx> vec;
        vec.emplace_back(SDL_Texture_ctx::IMG_Load(renderer, filename));
        auto [newit, inserted] = file_textures.emplace(filename, std::move(vec));
        return newit->second[0];
    }
}

MainWindow::operator SDL_Renderer_ctx&() {
    return renderer;
}

MainWindow::operator SDL_Renderer*() {
    return renderer;
}

void MainWindow::ChangeScreen(std::unique_ptr<Screen> NewScreen) {
    scr = std::move(NewScreen);
}

void MainWindow::Keyboard() {
    // message processing loop
    SDL_Event event;
    const auto& keysymr = event.key.keysym.sym;

    while(SDL_PollEvent(&event)) {
        // check for messages
        switch(event.type) {
        case SDL_KEYDOWN: {
            switch(keysymr) {
            case SDLK_ESCAPE:
                KEYS[static_cast<unsigned>(keysymr)] = true;
                break;
            default:
                break;
            }
            break;
        }
        case SDL_KEYUP: {
            switch(keysymr) {
            case SDLK_ESCAPE:
                KEYS[static_cast<unsigned>(keysymr)] = false;
                break;
            default:
                break;
            }
            break;
        }
        case SDL_WINDOWEVENT:
            if(event.window.event == SDL_WINDOWEVENT_CLOSE) {
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
    // Change the flag variable to 1 so that the main loop stops running
    quit = true;
}

int MainWindow::Width() const {
    return windim.x;
}
int MainWindow::Height() const {
    return windim.y;
}
const SDL_Point& MainWindow::GetWindowDimensions() const {
    return windim;
}
