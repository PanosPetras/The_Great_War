#include "SDL_ctx.h"

#include <stdexcept>

//-----------------------------------------------------------------------------
SDL_Init_ctx::SDL_Init_ctx() {
    if(not SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) throw std::runtime_error(std::string("SDL_Init_ctx: ") + SDL_GetError());
}

SDL_Init_ctx::~SDL_Init_ctx() {
    SDL_Quit();
}
//-----------------------------------------------------------------------------
TTF_Init_ctx::TTF_Init_ctx() {
    if(not TTF_Init()) throw std::runtime_error(std::string("TTF_Init_ctx: ") + SDL_GetError());
}

TTF_Init_ctx::~TTF_Init_ctx() {
    TTF_Quit();
}
//-----------------------------------------------------------------------------
SDL_Audio_ctx::SDL_Audio_ctx() : device(SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr)) {
    if(device == 0) throw std::runtime_error(std::string("SDL_Audio_ctx: ") + SDL_GetError());

    // Harmless if it is already playing, and spares us depending on the default
    SDL_ResumeAudioDevice(device);
}

SDL_Audio_ctx::~SDL_Audio_ctx() {
    SDL_CloseAudioDevice(device);
}
//=============================================================================
// SDL
//=============================================================================
SDL_Window_ctx::SDL_Window_ctx() :
    window(SDL_CreateWindow("The Great War", // window title
                            1920,           // width, in pixels
                            1080,           // height, in pixels
                            SDL_WINDOW_OPENGL //| SDL_WINDOW_FULLSCREEN            // flags - see below
                            ),
           &SDL_DestroyWindow)
// 2560x1440, 1920x1080, 1280x720
{
    if(not window) throw std::runtime_error("SDL_Window_ctx");
    // Get dimensions of the screen
    ::SDL_GetWindowSize(*this, &windim.x, &windim.y);
}

bool SDL_Window_ctx::SetFullScreen(bool on) {
    return ::SDL_SetWindowFullscreen(*this, on);
}

bool SDL_Window_ctx::SetSize(int width, int height) {
    if(width < 1 || height < 1) return false;
    if(not ::SDL_SetWindowSize(*this, width, height)) return false;
    ::SDL_GetWindowSize(*this, &windim.x, &windim.y);
    return width == windim.x && height == windim.y;
}

SDL_Window* SDL_Window_ctx::operator->() {
    return window.get();
}
SDL_Window_ctx::operator SDL_Window*() {
    return window.get();
}
//-----------------------------------------------------------------------------
/*SDL3 picks the driver itself - there is no accelerated flag to ask for, and
vsync moved off creation onto the renderer, so it is set right afterwards.*/
SDL_Renderer_ctx::SDL_Renderer_ctx(SDL_Window_ctx& window) : renderer(SDL_CreateRenderer(window, nullptr), &SDL_DestroyRenderer) {
    if(not renderer) throw std::runtime_error(std::string("SDL_Renderer_ctx: ") + SDL_GetError());
    SetVSync(true);
}

bool SDL_Renderer_ctx::SetVSync(bool on) {
    return ::SDL_SetRenderVSync(*this, on ? 1 : SDL_RENDERER_VSYNC_DISABLED);
}

SDL_Renderer* SDL_Renderer_ctx::operator->() {
    return renderer.get();
}
SDL_Renderer_ctx::operator SDL_Renderer*() {
    return renderer.get();
}
//-----------------------------------------------------------------------------
inline static SDL_Surface* Check(const std::string& txt, SDL_Surface* p) {
    if(not p) throw std::runtime_error("SDL_Surface_ctx::" + txt);
    return p;
}

SDL_Surface_ctx::SDL_Surface_ctx() : surface(nullptr, &SDL_DestroySurface) {}
SDL_Surface_ctx::SDL_Surface_ctx(SDL_Surface* s) : // private...
    surface(s, &SDL_DestroySurface)                   // ...so no Check needed
{}

// The two operators need to be check since we need to be able to default construct
// a SDL_Surface_ctx because of the threaded loading.
SDL_Surface* SDL_Surface_ctx::operator->() {
    return Check("operator->", surface.get());
}
SDL_Surface_ctx::operator SDL_Surface*() {
    return Check("operator SDL_Surface*", surface.get());
}

SDL_Surface_ctx SDL_Surface_ctx::IMG_Load(const std::string& filename) {
    return Check("IMG_Load(" + filename + ')', ::IMG_Load(filename.c_str()));
}
SDL_Surface_ctx SDL_Surface_ctx::TTF_RenderText_Blended(TTF_Font_ctx& font, const std::string& text, SDL_Color fg) {
    return Check("TTF_RenderText_Blended", ::TTF_RenderText_Blended(font, text.data(), text.size(), fg));
}
SDL_Surface_ctx SDL_Surface_ctx::TTF_RenderText_Blended_Wrapped(TTF_Font_ctx& font, const std::string& text, SDL_Color fg, Uint32 wrapLength) {
    return Check("TTF_RenderText_Blended_Wrapped", ::TTF_RenderText_Blended_Wrapped(font, text.data(), text.size(), fg, static_cast<int>(wrapLength)));
}
//-----------------------------------------------------------------------------
SDL_Cursor_ctx::SDL_Cursor_ctx() :
    cursor(
        [] {
            auto surface = SDL_Surface_ctx::IMG_Load("Icons/mouse.png");
            return ::SDL_CreateColorCursor(surface, 1, 1);
        }(),
        &SDL_DestroyCursor) {
    if(not cursor) throw std::runtime_error("SDL_Cursor_ctx");
    SDL_SetCursor(*this);
}

SDL_Cursor* SDL_Cursor_ctx::operator->() {
    return cursor.get();
}
SDL_Cursor_ctx::operator SDL_Cursor*() {
    return cursor.get();
}
//-----------------------------------------------------------------------------
SDL_Texture_ctx::SDL_Texture_ctx() : texture(nullptr, &SDL_DestroyTexture) {}
SDL_Texture_ctx::SDL_Texture_ctx(SDL_Renderer_ctx& r, SDL_Surface_ctx& s) : texture(SDL_CreateTextureFromSurface(r, s), &SDL_DestroyTexture) {
    if(not texture) throw std::runtime_error("SDL_Texture_ctx");
}

SDL_Texture* SDL_Texture_ctx::operator->() {
    return texture.get();
}
SDL_Texture_ctx::operator SDL_Texture*() {
    return texture.get();
}

SDL_Texture_ctx SDL_Texture_ctx::IMG_Load(SDL_Renderer_ctx& r, const std::string& filename) {
    auto surface = SDL_Surface_ctx::IMG_Load(filename);
    return {r, surface};
}
//=============================================================================
// TTF
//=============================================================================
TTF_Font_ctx::TTF_Font_ctx(int ptsize) : TTF_Font_ctx(DefaultFile, ptsize) {}
TTF_Font_ctx::TTF_Font_ctx(const std::string& filename, int ptsize) : font(TTF_OpenFont(filename.data(), ptsize), &TTF_CloseFont) {
    if(not font) throw std::runtime_error("TTF_Font_ctx");
}

TTF_Font* TTF_Font_ctx::operator->() {
    return font.get();
}
TTF_Font_ctx::operator TTF_Font*() {
    return font.get();
}
//=============================================================================
// Audio
//=============================================================================
SDL_Sound_ctx::SDL_Sound_ctx() : stream(nullptr, &SDL_DestroyAudioStream) {}

SDL_Sound_ctx::SDL_Sound_ctx(SDL_Audio_ctx& audio, const std::string& filename) : buffer(nullptr), stream(nullptr, &SDL_DestroyAudioStream) {
    SDL_AudioSpec spec;
    Uint8* buf = nullptr;

    if(not SDL_LoadWAV(filename.c_str(), &spec, &buf, &length)) {
        throw std::runtime_error("SDL_Sound_ctx: SDL_LoadWAV(" + filename + "): " + SDL_GetError());
    }
    buffer.reset(buf);

    /*Binding the stream to the device is what makes SDL convert the sound to
    whatever format the device actually wants, so the source format is all we
    have to describe here.*/
    stream.reset(SDL_CreateAudioStream(&spec, &spec));
    if(not stream) throw std::runtime_error(std::string("SDL_Sound_ctx: SDL_CreateAudioStream: ") + SDL_GetError());

    if(not SDL_BindAudioStream(audio.Device(), stream.get())) {
        throw std::runtime_error(std::string("SDL_Sound_ctx: SDL_BindAudioStream: ") + SDL_GetError());
    }
}

bool SDL_Sound_ctx::Play() {
    if(not stream) return false;

    // Drop whatever is still queued, so a rapid second click restarts the sound
    if(not SDL_ClearAudioStream(stream.get())) return false;

    return SDL_PutAudioStreamData(stream.get(), buffer.get(), static_cast<int>(length));
}
//-----------------------------------------------------------------------------
bool RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& dst) {
    const SDL_FRect frect = ToFRect(dst);
    return SDL_RenderTexture(renderer, texture, nullptr, &frect);
}
//-----------------------------------------------------------------------------
