#include "SDL_ctx.h"

//-----------------------------------------------------------------------------
SDL_Init_ctx::SDL_Init_ctx() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}

SDL_Init_ctx::~SDL_Init_ctx() {
    SDL_Quit();
}
//-----------------------------------------------------------------------------
SDL_Window_ctx::SDL_Window_ctx() : window(SDL_CreateWindow(
        "The Great War",                  // window title
        SDL_WINDOWPOS_UNDEFINED,          // initial x position
        SDL_WINDOWPOS_UNDEFINED,          // initial y position
        1920,                             // width, in pixels
        1080,                             // height, in pixels
        SDL_WINDOW_OPENGL //| SDL_WINDOW_FULLSCREEN                  // flags - see below
    ))
    //2560x1440, 1920x1080, 1280x720
{
    //Get dimensions of the screen
    SDL_GetWindowSize(window, &width, &height);
}

SDL_Window_ctx::~SDL_Window_ctx() {
    SDL_DestroyWindow(window);
}

SDL_Window* SDL_Window_ctx::operator->() { return window; }

SDL_Window_ctx::operator SDL_Window* () { return window; }
//-----------------------------------------------------------------------------
SDL_Renderer_ctx::SDL_Renderer_ctx(SDL_Window_ctx& window) : renderer(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) {}

SDL_Renderer_ctx::~SDL_Renderer_ctx() {
    SDL_DestroyRenderer(renderer);
}

SDL_Renderer* SDL_Renderer_ctx::operator->() { return renderer; }

SDL_Renderer_ctx::operator SDL_Renderer* () { return renderer; }
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
SDL_Cursor_ctx::SDL_Cursor_ctx() {
    SDL_Surface* surface = IMG_Load("Icons/mouse.png");
    cursor = SDL_CreateColorCursor(surface, 1, 1);
    SDL_SetCursor(cursor);
    SDL_FreeSurface(surface);
}

SDL_Cursor_ctx::~SDL_Cursor_ctx() {
    SDL_FreeCursor(cursor);
}

SDL_Cursor* SDL_Cursor_ctx::operator->() { return cursor; }

SDL_Cursor_ctx::operator SDL_Cursor* () { return cursor; }
//-----------------------------------------------------------------------------
