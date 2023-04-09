#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_thread.h>
#include <SDL_ttf.h>

class SDL_Init_ctx {
public:
    SDL_Init_ctx();
    SDL_Init_ctx(const SDL_Init_ctx&) = delete;
    SDL_Init_ctx(SDL_Init_ctx&&) = delete;
    SDL_Init_ctx& operator=(const SDL_Init_ctx&) = delete;
    SDL_Init_ctx& operator=(SDL_Init_ctx&&) = delete;
    ~SDL_Init_ctx();
};

class SDL_Window_ctx {
public:
    SDL_Window_ctx();
    SDL_Window_ctx(const SDL_Window_ctx&) = delete;
    SDL_Window_ctx(SDL_Window_ctx&&) = delete;
    SDL_Window_ctx& operator=(const SDL_Window_ctx&) = delete;
    SDL_Window_ctx& operator=(SDL_Window_ctx&&) = delete;
    ~SDL_Window_ctx();

    SDL_Window* operator->();
    operator SDL_Window* ();
private:
    SDL_Window* window;
};

class SDL_Renderer_ctx {
public:
    explicit SDL_Renderer_ctx(SDL_Window_ctx& window);
    SDL_Renderer_ctx(const SDL_Renderer_ctx&) = delete;
    SDL_Renderer_ctx(SDL_Renderer_ctx&&) = delete;
    SDL_Renderer_ctx& operator=(const SDL_Renderer_ctx&) = delete;
    SDL_Renderer_ctx& operator=(SDL_Renderer_ctx&&) = delete;
    ~SDL_Renderer_ctx();

    SDL_Renderer* operator->();
    operator SDL_Renderer* ();
private:
    SDL_Renderer* renderer;
};

class TTF_Init_ctx {
public:
    TTF_Init_ctx();
    TTF_Init_ctx(const TTF_Init_ctx&) = delete;
    TTF_Init_ctx(TTF_Init_ctx&&) = delete;
    TTF_Init_ctx& operator=(const TTF_Init_ctx&) = delete;
    TTF_Init_ctx& operator=(TTF_Init_ctx&&) = delete;
    ~TTF_Init_ctx();
};

class IMG_Init_ctx {
public:
    IMG_Init_ctx();
    IMG_Init_ctx(const IMG_Init_ctx&) = delete;
    IMG_Init_ctx(IMG_Init_ctx&&) = delete;
    IMG_Init_ctx& operator=(const IMG_Init_ctx&) = delete;
    IMG_Init_ctx& operator=(IMG_Init_ctx&&) = delete;
    ~IMG_Init_ctx();
};

class MIX_ctx {
public:
    MIX_ctx();
    MIX_ctx(const MIX_ctx&) = delete;
    MIX_ctx(MIX_ctx&&) = delete;
    MIX_ctx& operator=(const MIX_ctx&) = delete;
    MIX_ctx& operator=(MIX_ctx&&) = delete;
    ~MIX_ctx();
};

class SDL_Cursor_ctx {
public:
    SDL_Cursor_ctx();
    SDL_Cursor_ctx(const SDL_Cursor_ctx&) = delete;
    SDL_Cursor_ctx(SDL_Cursor_ctx&&) = delete;
    SDL_Cursor_ctx& operator=(const SDL_Cursor_ctx&) = delete;
    SDL_Cursor_ctx& operator=(SDL_Cursor_ctx&&) = delete;
    ~SDL_Cursor_ctx();

    SDL_Cursor* operator->();
    operator SDL_Cursor* ();
private:
    SDL_Cursor* cursor;
};
