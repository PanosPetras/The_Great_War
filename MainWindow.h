#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Screen.h"

#include "SDL_ctx.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

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

    SDL_Texture_ctx& IMG_Load(const std::string& filename);

    template<class... Mods>
    std::array<TextureRef, sizeof...(Mods)> IMG_Load(const std::string& filename, Mods&&... mods) {
        auto vecit = [&]{
            if(auto it = file_textures.find(filename); it != file_textures.end()) {
                std::cerr << "Cached load of " << filename << std::endl;
                if(it->second.size() != sizeof...(Mods)) {
                    // NOTE: This can happen if it was via an Image that only cares about one state.
                    // If that happens, either we need to create duplicate textures - or we preload them all.
                    std::cerr << "Mismatch between cached textures and number of Mods" << std::endl;
                    std::terminate();
                }
                return it;
            }
            // Not found in cache
            std::cerr << "First load of " << filename << std::endl;
            auto surface = SDL_Surface_ctx::IMG_Load(filename);

            // a functor to create a texture from the surface and apply a modification
            auto dupmod = [&](auto&& mod) {
                SDL_Texture_ctx rv(renderer, surface);
                mod(rv);
                return rv;
            };

            std::vector<SDL_Texture_ctx> txts;
            // create textures from the surface and apply modifications
            (..., txts.emplace_back(dupmod(mods)));

            auto [newit, inserted] = file_textures.emplace(filename, std::move(txts));

            return newit;
        }();

        // Return the array of TextureRefs
        return [&textures=vecit->second]<std::size_t... I>(std::index_sequence<I...>) -> std::array<TextureRef, sizeof...(I)> {
            return {TextureRef(textures[I])...};
        }(std::make_index_sequence<sizeof...(Mods)>{});
    }

    operator SDL_Renderer* ();
    operator SDL_Renderer_ctx& ();

    int Width() const;
    int Height() const;
    const SDL_Point& GetWindowDimensions() const;

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

    std::array<bool, 322> KEYS{};  // 322 is the number of SDLK_DOWN events

    bool quit = false;

    //Used to store the window's width and Height:
    SDL_Point windim{};
    //Used to store some of the window's settings
    bool vsync;
    bool fullscreen;

    std::vector<std::function<void()>> event_queue; // deferred events
    std::unordered_map<std::string, std::vector<SDL_Texture_ctx>> file_textures;

    //Stores a pointer to the active screen - should be last since it
    //uses the main window resources that need to be initialized first.
    std::unique_ptr<Screen> scr;

    friend class MenuSettingsScreen;
};

#endif
