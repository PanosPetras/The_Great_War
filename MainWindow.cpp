#include "MainWindow.h"

#include "Button.h"

#include "Resolutions.h"
#include "ScreenList.h"

#include <iostream>
#include <tuple>

MainWindow& MainWindow::Instance() {
    static MainWindow inst;
    return inst;
}

MainWindow::MainWindow() : renderer(window), scr(std::make_unique<MainMenu>(*this, [this] { Quit(); }, [this](std::unique_ptr<Screen> newScreen) { ChangeScreen(std::move(newScreen)); })) {
    vsync = true;
    fullscreen = true;
    framerateCap = 60;
}

void MainWindow::MainLoop() {
    Uint64 previous = SDL_GetTicks();

    while(!quit) {
        const Uint64 now = SDL_GetTicks();
        const Uint32 elapsed = static_cast<Uint32>(now - previous);

        /*With vsync on, SDL_RenderPresent does the waiting for us. Without it
        we have to honour the framerate cap ourselves, and yield rather than
        spin so that we are not burning a core doing nothing.*/
        if(not vsync && elapsed < 1000u / static_cast<Uint32>(framerateCap)) {
            SDL_Delay(1);
            continue;
        }
        previous = now;

        /*Advance the game by the time this frame took before drawing it. The
        simulation therefore runs on this thread, which is what makes it safe
        to read game state while rendering.*/
        scr->Update(elapsed);

        Render();
        Keyboard();
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
    const auto& scancode = event.key.scancode;

    while(SDL_PollEvent(&event)) {
        // check for messages
        switch(event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch(scancode) {
            case SDL_SCANCODE_ESCAPE:
                KEYS[scancode] = true;
                break;
            default:
                break;
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            switch(scancode) {
            case SDL_SCANCODE_ESCAPE:
                KEYS[scancode] = false;
                break;
            default:
                break;
            }
            break;
        }
        /*SDL3 split SDL_WINDOWEVENT into an event type per window event, so
        the close request arrives as its own type rather than as a field.*/
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            Quit();
            break;
        // exit if the window is closed
        case SDL_EVENT_QUIT:
            KEYS[SDL_SCANCODE_ESCAPE] = true;
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

const SDL_Point& MainWindow::GetWindowDimensions() const {
    return window.GetWindowDimensions();
}
int MainWindow::Width() const {
    return GetWindowDimensions().x;
}
int MainWindow::Height() const {
    return GetWindowDimensions().y;
}

bool MainWindow::SetResolution(unsigned resolution, bool Vsync, bool Fullscreen) {
    auto& res = Resolutions::SUPPORTED_RESOLUTIONS.at(resolution);

    if(window.SetFullScreen(Fullscreen) == false) {
        std::cerr << "MainWindow::SetResolution window.SetFullScreen " << Fullscreen << " failed\n";
        return false;
    }
    if(renderer.SetVSync(Vsync) == false) {
        std::cerr << "MainWindow::SetResolution renderer.SetVSync " << Vsync << " failed\n";
        return false;
    }
    if(window.SetSize(res.GetWidth(), res.GetHeight()) == false) {
        std::cerr << "MainWindow::SetResolution window.SetSize " << res.GetWidth() << ',' << res.GetHeight() << " failed\n";
        return false;
    }

    vsync = Vsync;
    fullscreen = Fullscreen;
    framerateCap = Resolutions::SUPPORTED_FRAMERATES[resolution];

    return true;
}

FontRef MainWindow::TTF_OpenFont(FontSize size) {
    return TTF_OpenFont(Pt(size, Height()));
}

FontRef MainWindow::TTF_OpenFont(int ptsize) {
    return TTF_OpenFont(TTF_Font_ctx::DefaultFile, ptsize);
}

FontRef MainWindow::TTF_OpenFont(const std::string& filename, int ptsize) {
    auto key = std::pair(filename, ptsize);

    if(auto it = file_fonts.find(key); it != file_fonts.end()) {
        return FontRef(it->second);
    }

    auto [newit, inserted] = file_fonts.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(key)), std::forward_as_tuple(filename, ptsize));
    return FontRef(newit->second);
}

SoundRef MainWindow::LoadSound(const std::string& filename) {
    if(auto it = file_sounds.find(filename); it != file_sounds.end()) {
        return &it->second;
    }
    auto [newit, inserted] = file_sounds.emplace(std::piecewise_construct, std::forward_as_tuple(filename), std::forward_as_tuple(audio_ctx, filename));
    return &newit->second;
}
