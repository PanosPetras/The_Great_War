#include "SDL_ctx.h"

//-----------------------------------------------------------------------------
SDL_Init_ctx::SDL_Init_ctx() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}

SDL_Init_ctx::~SDL_Init_ctx() {
    SDL_Quit();
}
//-----------------------------------------------------------------------------
TTF_Init_ctx::TTF_Init_ctx() {
    TTF_Init();
}

TTF_Init_ctx::~TTF_Init_ctx() {
    TTF_Quit();
}
//-----------------------------------------------------------------------------
IMG_Init_ctx::IMG_Init_ctx() {
    IMG_Init(0);
}

IMG_Init_ctx::~IMG_Init_ctx() {
    IMG_Quit();
}
//-----------------------------------------------------------------------------
MIX_ctx::MIX_ctx() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_AllocateChannels(2);
}

MIX_ctx::~MIX_ctx() {
    Mix_Quit();
}
//-----------------------------------------------------------------------------
SDL_Window_ctx::SDL_Window_ctx() : window(SDL_CreateWindow(
        "The Great War",                  // window title
        SDL_WINDOWPOS_UNDEFINED,          // initial x position
        SDL_WINDOWPOS_UNDEFINED,          // initial y position
        1920,                             // width, in pixels
        1080,                             // height, in pixels
        SDL_WINDOW_OPENGL //| SDL_WINDOW_FULLSCREEN                  // flags - see below
    ), &SDL_DestroyWindow)
    //2560x1440, 1920x1080, 1280x720
{
    //Get dimensions of the screen
    SDL_GetWindowSize(*this, &windim.x, &windim.y);
}

SDL_Window* SDL_Window_ctx::operator->() { return window.get(); }
SDL_Window_ctx::operator SDL_Window* () { return window.get(); }
//-----------------------------------------------------------------------------
SDL_Renderer_ctx::SDL_Renderer_ctx(SDL_Window_ctx& window) :
    renderer(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                            SDL_RENDERER_PRESENTVSYNC),
             &SDL_DestroyRenderer)
{}

SDL_Renderer* SDL_Renderer_ctx::operator->() { return renderer.get(); }
SDL_Renderer_ctx::operator SDL_Renderer* () { return renderer.get(); }
//-----------------------------------------------------------------------------
SDL_Cursor_ctx::SDL_Cursor_ctx() :
    cursor(SDL_CreateColorCursor(SDL_Surface_ctx(IMG_Load("Icons/mouse.png")), 1, 1), &SDL_FreeCursor)
{
    SDL_SetCursor(*this);
}

SDL_Cursor* SDL_Cursor_ctx::operator->() { return cursor.get(); }
SDL_Cursor_ctx::operator SDL_Cursor* () { return cursor.get(); }
//-----------------------------------------------------------------------------
SDL_Surface_ctx::SDL_Surface_ctx() : SDL_Surface_ctx(nullptr) {}
SDL_Surface_ctx::SDL_Surface_ctx(SDL_Surface* s) : surface(s, &SDL_FreeSurface) {}

SDL_Surface* SDL_Surface_ctx::operator->() { return surface.get(); }
SDL_Surface_ctx::operator SDL_Surface* () { return surface.get(); }
SDL_Surface_ctx::operator const SDL_Surface* () const { return surface.get(); }

SDL_Surface_ctx SDL_Surface_ctx::IMG_Load(std::string_view filename) {
    return SDL_Surface_ctx(::IMG_Load(filename.data()));
}
//-----------------------------------------------------------------------------
std::unordered_map<std::string, SDL_Texture_ctx> SDL_Texture_ctx::textureCache;
SDL_Texture_ctx::SDL_Texture_ctx() : texture(nullptr, &SDL_DestroyTexture) {}
SDL_Texture_ctx::SDL_Texture_ctx(SDL_Renderer_ctx& r, SDL_Surface_ctx& s) :
    texture(SDL_CreateTextureFromSurface(r, s), &SDL_DestroyTexture)
{}

SDL_Texture* SDL_Texture_ctx::operator->() { return texture.get(); }
SDL_Texture_ctx::operator SDL_Texture* () { return texture.get(); }

SDL_Texture_ctx::SDL_Texture_ctx(SDL_Renderer_ctx& r, SDL_Texture_ctx& t, SDL_Color c) :
    texture(t.texture) {
    SDL_SetTextureColorMod(texture.get(), c.r, c.g, c.b);
}

SDL_Texture_ctx SDL_Texture_ctx::IMG_Load(SDL_Renderer_ctx& r, std::string_view filename) {
    return IMG_Load_ColorMod(r, filename, SDL_Color{.r = 255, .g = 255, .b = 255 });
}

std::string SDL_Color_to_string(SDL_Color c) {
    return std::to_string(c.r) + "/" + std::to_string(c.g) + "/" + std::to_string(c.b);
}

SDL_Texture_ctx SDL_Texture_ctx::IMG_Load_ColorMod(SDL_Renderer_ctx& r, std::string_view filename, SDL_Color color) {
    std::string keyName = std::string(filename) + SDL_Color_to_string(color);

    if (!textureCache.contains(keyName)) {
        auto surface = SDL_Surface_ctx::IMG_Load(filename);

        textureCache[keyName] = SDL_Texture_ctx(r, surface);
    }

    return { r, textureCache.find(keyName)->second, color };
}
//-----------------------------------------------------------------------------
TTF_Font_ctx::TTF_Font_ctx(int ptsize) : TTF_Font_ctx("Fonts/segoeui.ttf", ptsize) {}
TTF_Font_ctx::TTF_Font_ctx(std::string_view filename, int ptsize) :
    font(TTF_OpenFont(filename.data(), ptsize), &TTF_CloseFont)
{}

TTF_Font* TTF_Font_ctx::operator->() { return font.get(); }
TTF_Font_ctx::operator TTF_Font* () { return font.get(); }
//-----------------------------------------------------------------------------
