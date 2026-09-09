#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core/FontSize.h"
#include "screens/Screen.h"

#include "core/SDL_ctx.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

class MainWindow {
    // A cached image, identified by its file and by how many variants of it were made
    using TextureKey = std::pair<std::string, std::size_t>;

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

    // Loads a file once and hands out the single, unmodified texture from it
    SDL_Texture_ctx& IMG_Load(const std::string& filename);

    /*Loads a file once and keeps one texture per modification handed in - the
    idle, hovered and inactive tints a button wants, say. The cache is keyed by
    file *and* by how many variants were asked for, so a caller wanting three
    tinted copies and one wanting a single plain texture get separate entries
    rather than one of them finding the other's and mistaking it for its own.
    An entry is therefore only ever built once and never grown, which is what
    keeps the references handed out of it valid for the life of the window.*/
    template<class... Mods>
    std::array<TextureRef, sizeof...(Mods)> IMG_Load(const std::string& filename, Mods&&... mods) {
        auto key = TextureKey(filename, sizeof...(Mods));

        auto it = file_textures.find(key);
        if(it == file_textures.end()) {
            auto surface = SDL_Surface_ctx::IMG_Load(filename);

            // a functor to create a texture from the surface and apply a modification
            auto dupmod = [&](auto&& mod) {
                SDL_Texture_ctx rv(renderer, surface);
                mod(rv);
                return rv;
            };

            std::vector<SDL_Texture_ctx> txts;
            txts.reserve(sizeof...(Mods));
            // create textures from the surface and apply modifications
            (..., txts.emplace_back(dupmod(mods)));

            it = file_textures.emplace(std::move(key), std::move(txts)).first;
        }

        // Return the array of TextureRefs
        return [&textures = it->second]<std::size_t... I>(std::index_sequence<I...>) -> std::array<TextureRef, sizeof...(I)> {
            return {TextureRef(textures[I])...};
        }(std::make_index_sequence<sizeof...(Mods)>{});
    }

    // Hands out a sound kept loaded for the lifetime of the window
    SoundRef LoadSound(const std::string& filename);

    /*Hands out a font kept open for the lifetime of the window. A TTF_Font
    bakes its point size in at open time, so there is one per (file, size)
    rather than one overall - but the handful of sizes the UI uses are opened
    once each, instead of on every single text change.*/
    FontRef TTF_OpenFont(FontSize size);
    FontRef TTF_OpenFont(int ptsize);
    FontRef TTF_OpenFont(const std::string& filename, int ptsize);

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
    SDL_Audio_ctx audio_ctx;
    SDL_Cursor_ctx cursor; //The window's cursor

    /*Indexed by scancode. SDL2's keycodes were small enough to index an array
    with; SDL3's are Unicode codepoints for printable keys and carry a high bit
    otherwise, so a keycode is no longer an array index. Scancodes still are,
    and SDL_SCANCODE_COUNT is how many there are.*/
    std::array<bool, SDL_SCANCODE_COUNT> KEYS{};

    bool quit = false;

    //Used to store some of the window's settings
    bool vsync;
    bool fullscreen;
    int framerateCap;
    bool SetResolution(unsigned resolution, bool vsync, bool fullscreen);

    std::vector<std::function<void()>> event_queue; // deferred events
    std::map<TextureKey, std::vector<SDL_Texture_ctx>> file_textures;
    // Declared after audio_ctx so that every sound is torn down before the device
    std::unordered_map<std::string, SDL_Sound_ctx> file_sounds;
    // Declared after ttf_init_ctx so that every font is closed before TTF_Quit
    std::map<std::pair<std::string, int>, TTF_Font_ctx> file_fonts;

    //Stores a pointer to the active screen - should be last since it
    //uses the main window resources that need to be initialized first.
    std::unique_ptr<Screen> scr;

    friend class MenuSettingsScreen;
};

#endif
