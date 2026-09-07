#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <stdexcept>
#include <string>

/*-----------------------------------------------------------------------------
The promiscuous_ref is to be used while converting from storing
pointers to SDL objects to storing instances of RAII SDL wrappers.
It's a reference- and non-owning- pointer wrapper in one.

  * It can be implicitly converted to a reference to an instance of the RAII
    class.

  * operator->() returns a pointer to the RAII class.

  * It can be implicitly converted into a pointer to the object the RAII
    instance encapsulates.
-----------------------------------------------------------------------------*/
template<class T, class U>
class promiscuous_ref {
public:
    promiscuous_ref() noexcept = default;

    explicit promiscuous_ref(T& o) noexcept : obj(&o) {}
    promiscuous_ref& operator=(T& o) noexcept {
        obj = &o;
        return *this;
    }

    promiscuous_ref(const promiscuous_ref& other) noexcept = default;
    promiscuous_ref(promiscuous_ref&& other) = delete;
    promiscuous_ref& operator=(const promiscuous_ref& other) noexcept = default;
    promiscuous_ref& operator=(promiscuous_ref&& other) = delete;
    ~promiscuous_ref() = default;

    operator T&() {
        if(obj == nullptr) throw std::runtime_error("promiscuous_ref::operator T& (): nullptr dereferenced");
        return *obj;
    }
    operator const T&() const {
        if(obj == nullptr) throw std::runtime_error("promiscuous_ref::operator const T& () const: nullptr dereferenced");
        return *obj;
    }

    T* operator->() {
        if(obj == nullptr) throw std::runtime_error("T* promiscuous_ref::operator->(): nullptr returned");
        return obj;
    }
    const T* operator->() const {
        if(obj == nullptr) throw std::runtime_error("const T* promiscuous_ref::operator->() const: nullptr returned");
        return obj;
    }

    operator U*() {
        if(obj == nullptr) throw std::runtime_error("promiscuous_ref::operator U* (): nullptr returned");
        return *obj;
    }
    operator const U*() const {
        if(obj == nullptr) throw std::runtime_error("promiscuous_ref::operator const U* () const: nullptr returned");
        return *obj;
    }

private:
    T* obj = nullptr;
};
//-----------------------------------------------------------------------------
// classes not owning any actual resources
class SDL_Init_ctx {
public:
    SDL_Init_ctx();
    SDL_Init_ctx(const SDL_Init_ctx&) = delete;
    SDL_Init_ctx(SDL_Init_ctx&&) noexcept = default;
    SDL_Init_ctx& operator=(const SDL_Init_ctx&) = delete;
    SDL_Init_ctx& operator=(SDL_Init_ctx&&) noexcept = default;
    ~SDL_Init_ctx();
};

class TTF_Init_ctx {
public:
    TTF_Init_ctx();
    TTF_Init_ctx(const TTF_Init_ctx&) = delete;
    TTF_Init_ctx(TTF_Init_ctx&&) noexcept = default;
    TTF_Init_ctx& operator=(const TTF_Init_ctx&) = delete;
    TTF_Init_ctx& operator=(TTF_Init_ctx&&) noexcept = default;
    ~TTF_Init_ctx();
};

class IMG_Init_ctx {
public:
    IMG_Init_ctx();
    IMG_Init_ctx(const IMG_Init_ctx&) = delete;
    IMG_Init_ctx(IMG_Init_ctx&&) noexcept = default;
    IMG_Init_ctx& operator=(const IMG_Init_ctx&) = delete;
    IMG_Init_ctx& operator=(IMG_Init_ctx&&) noexcept = default;
    ~IMG_Init_ctx();
};

/*The playback device every sound is mixed into. SDL mixes the streams bound
to a device for us, so this replaces what SDL_mixer's channels used to do.*/
class SDL_Audio_ctx {
public:
    SDL_Audio_ctx();
    SDL_Audio_ctx(const SDL_Audio_ctx&) = delete;
    SDL_Audio_ctx(SDL_Audio_ctx&&) noexcept = default;
    SDL_Audio_ctx& operator=(const SDL_Audio_ctx&) = delete;
    SDL_Audio_ctx& operator=(SDL_Audio_ctx&&) noexcept = default;
    ~SDL_Audio_ctx();

    inline SDL_AudioDeviceID Device() const { return device; }

private:
    SDL_AudioDeviceID device;
};
//-----------------------------------------------------------------------------
class TTF_Font_ctx {
public:
    // The font every part of the UI draws with, unless it asks for another one
    inline static constexpr const char* DefaultFile = "Fonts/segoeui.ttf";

    TTF_Font_ctx(int ptsize); // use the default
    explicit TTF_Font_ctx(const std::string& filename, int ptsize);

    TTF_Font_ctx(const TTF_Font_ctx&) = delete;
    TTF_Font_ctx(TTF_Font_ctx&&) noexcept = default;
    TTF_Font_ctx& operator=(const TTF_Font_ctx&) = delete;
    TTF_Font_ctx& operator=(TTF_Font_ctx&&) noexcept = default;
    ~TTF_Font_ctx() = default;

    TTF_Font* operator->();
    operator TTF_Font*();

private:
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font;
};
using FontRef = promiscuous_ref<TTF_Font_ctx, TTF_Font>;
//-----------------------------------------------------------------------------
class SDL_Window_ctx {
public:
    SDL_Window_ctx();
    SDL_Window_ctx(const SDL_Window_ctx&) = delete;
    SDL_Window_ctx(SDL_Window_ctx&&) noexcept = default;
    SDL_Window_ctx& operator=(const SDL_Window_ctx&) = delete;
    SDL_Window_ctx& operator=(SDL_Window_ctx&&) noexcept = default;
    ~SDL_Window_ctx() = default;

    inline const SDL_Point& GetWindowDimensions() const { return windim; }
    /*SDL3 has a single fullscreen mode - a window is either fullscreen at the
    display's current mode or it is not, so this takes a bool rather than the
    SDL_WINDOW_FULLSCREEN / SDL_WINDOW_FULLSCREEN_DESKTOP flags SDL2 wanted.*/
    bool SetFullScreen(bool on);
    bool SetSize(int width, int height);

    SDL_Window* operator->();
    operator SDL_Window*();

private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    SDL_Point windim;
};

class SDL_Renderer_ctx {
public:
    explicit SDL_Renderer_ctx(SDL_Window_ctx& window);
    SDL_Renderer_ctx(const SDL_Renderer_ctx&) = delete;
    SDL_Renderer_ctx(SDL_Renderer_ctx&&) noexcept = default;
    SDL_Renderer_ctx& operator=(const SDL_Renderer_ctx&) = delete;
    SDL_Renderer_ctx& operator=(SDL_Renderer_ctx&&) noexcept = default;
    ~SDL_Renderer_ctx() = default;

    bool SetVSync(bool on);

    SDL_Renderer* operator->();
    operator SDL_Renderer*();

private:
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
};
using RendererRef = promiscuous_ref<SDL_Renderer_ctx, SDL_Renderer>;

class SDL_Cursor_ctx {
public:
    SDL_Cursor_ctx();
    SDL_Cursor_ctx(const SDL_Cursor_ctx&) = delete;
    SDL_Cursor_ctx(SDL_Cursor_ctx&&) noexcept = default;
    SDL_Cursor_ctx& operator=(const SDL_Cursor_ctx&) = delete;
    SDL_Cursor_ctx& operator=(SDL_Cursor_ctx&&) noexcept = default;
    ~SDL_Cursor_ctx() = default;

    SDL_Cursor* operator->();
    operator SDL_Cursor*();

private:
    std::unique_ptr<SDL_Cursor, decltype(&SDL_DestroyCursor)> cursor;
};

class SDL_Surface_ctx {
public:
    SDL_Surface_ctx(); // non-owning
    SDL_Surface_ctx(const SDL_Surface_ctx&) = delete;
    SDL_Surface_ctx(SDL_Surface_ctx&&) noexcept = default;
    SDL_Surface_ctx& operator=(const SDL_Surface_ctx&) = delete;
    SDL_Surface_ctx& operator=(SDL_Surface_ctx&&) noexcept = default;
    ~SDL_Surface_ctx() = default;

    SDL_Surface* operator->();
    operator SDL_Surface*();
    inline explicit operator bool() const { return static_cast<bool>(surface); }

    static SDL_Surface_ctx IMG_Load(const std::string& filename);
    static SDL_Surface_ctx TTF_RenderText_Blended(TTF_Font_ctx& cont, const std::string& text, SDL_Color fg);
    static SDL_Surface_ctx TTF_RenderText_Blended_Wrapped(TTF_Font_ctx& font, const std::string& text, SDL_Color fg, Uint32 wrapLength);

private:
    SDL_Surface_ctx(SDL_Surface*); // take ownership of a raw pointer

    std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> surface;
};

class SDL_Texture_ctx {
public:
    SDL_Texture_ctx(); // non-owning
    SDL_Texture_ctx(SDL_Renderer_ctx&, SDL_Surface_ctx&);

    SDL_Texture_ctx(const SDL_Texture_ctx&) = delete;
    SDL_Texture_ctx(SDL_Texture_ctx&&) noexcept = default;
    SDL_Texture_ctx& operator=(const SDL_Texture_ctx&) = delete;
    SDL_Texture_ctx& operator=(SDL_Texture_ctx&&) noexcept = default;
    ~SDL_Texture_ctx() = default;

    inline explicit operator bool() const { return static_cast<bool>(texture); }
    SDL_Texture* operator->();
    operator SDL_Texture*();

    static SDL_Texture_ctx IMG_Load(SDL_Renderer_ctx& r, const std::string& filename);

private:
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
};
using TextureRef = promiscuous_ref<SDL_Texture_ctx, SDL_Texture>;
//-----------------------------------------------------------------------------
/*A sound effect, decoded once and kept in memory. Each one owns the stream it
plays through, which is what lets two sounds overlap without SDL_mixer's fixed
set of channels to allocate them out of.*/
class SDL_Sound_ctx {
public:
    SDL_Sound_ctx(); // no sound loaded
    SDL_Sound_ctx(SDL_Audio_ctx& audio, const std::string& filename);

    SDL_Sound_ctx(const SDL_Sound_ctx&) = delete;
    SDL_Sound_ctx(SDL_Sound_ctx&&) noexcept = default;
    SDL_Sound_ctx& operator=(const SDL_Sound_ctx&) = delete;
    SDL_Sound_ctx& operator=(SDL_Sound_ctx&&) noexcept = default;
    ~SDL_Sound_ctx() = default;

    /*Plays the sound from the start, cutting short whatever was left of a
    previous play of this same sound - the button click SDL_mixer used to
    restart on its one channel behaves the same way.*/
    bool Play();

private:
    struct Free {
        void operator()(Uint8* p) const { SDL_free(p); }
    };

    std::unique_ptr<Uint8, Free> buffer;
    Uint32 length = 0;
    std::unique_ptr<SDL_AudioStream, decltype(&SDL_DestroyAudioStream)> stream;
};
/*Sounds have no raw SDL counterpart to convert to any more, so a plain
non-owning pointer is all a reference to one needs to be.*/
using SoundRef = SDL_Sound_ctx*;
//-----------------------------------------------------------------------------
/*SDL3 draws into float rectangles. The UI lays itself out in whole pixels, so
these convert at the point of drawing rather than spreading floats through it.*/
inline SDL_FRect ToFRect(const SDL_Rect& r) {
    return SDL_FRect{static_cast<float>(r.x), static_cast<float>(r.y), static_cast<float>(r.w), static_cast<float>(r.h)};
}

// SDL_RenderCopy's replacement, taking the integer rectangle the UI holds
bool RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& dst);
//-----------------------------------------------------------------------------
