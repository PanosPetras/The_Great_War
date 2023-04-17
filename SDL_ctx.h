#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_thread.h>
#include <SDL_ttf.h>

#include <iostream>
#include <memory>
#include <string_view>

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
    promiscuous_ref& operator=(T& o) noexcept { obj = &o; return *this; }

    promiscuous_ref(const promiscuous_ref& other) noexcept = default;
    promiscuous_ref(promiscuous_ref&& other) = delete;
    promiscuous_ref& operator=(const promiscuous_ref& other) noexcept = default;
    promiscuous_ref& operator=(promiscuous_ref&& other) = delete;
    ~promiscuous_ref() = default;

    operator T& () {
        if(obj == nullptr) std::cerr << "promiscuous_ref nullptr dereferenced" << std::endl;
        return *obj;
    }
    operator const T& () const {
        if(obj == nullptr) std::cerr << "promiscuous_ref nullptr dereferenced" << std::endl;
        return *obj;
    }

    T* operator->() {
        if(obj == nullptr) {
            std::cerr << "promiscuous_ref nullptr returned" << std::endl;
            return nullptr;
        }
        return obj;
    }
    const T* operator->() const {
        if(obj == nullptr) {
            std::cerr << "promiscuous_ref nullptr returned" << std::endl;
            return nullptr;
        }
        return obj;
    }

    operator U* () {
        if(obj == nullptr) {
            std::cerr << "promiscuous_ref nullptr returned" << std::endl;
            return nullptr;
        }
        return *obj;
    }
    operator const U* () const {
        if(obj == nullptr) {
            std::cerr << "promiscuous_ref nullptr returned" << std::endl;
            return nullptr;
        }
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

class MIX_ctx {
public:
    MIX_ctx();
    MIX_ctx(const MIX_ctx&) = delete;
    MIX_ctx(MIX_ctx&&) noexcept = default;
    MIX_ctx& operator=(const MIX_ctx&) = delete;
    MIX_ctx& operator=(MIX_ctx&&) noexcept = default;
    ~MIX_ctx();
};
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

    SDL_Window* operator->();
    operator SDL_Window* ();
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

    SDL_Renderer* operator->();
    operator SDL_Renderer* ();
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
    operator SDL_Cursor* ();
private:
    std::unique_ptr<SDL_Cursor, decltype(&SDL_FreeCursor)> cursor;
};

class SDL_Surface_ctx {
public:
    SDL_Surface_ctx(); // non-owning
    explicit SDL_Surface_ctx(SDL_Surface*); // take ownership of a raw pointer

    SDL_Surface_ctx(const SDL_Surface_ctx&) = delete;
    SDL_Surface_ctx(SDL_Surface_ctx&&) noexcept = default;
    SDL_Surface_ctx& operator=(const SDL_Surface_ctx&) = delete;
    SDL_Surface_ctx& operator=(SDL_Surface_ctx&&) noexcept = default;
    ~SDL_Surface_ctx() = default;

    SDL_Surface* operator->();
    operator SDL_Surface* ();
    operator const SDL_Surface* () const;

    static SDL_Surface_ctx IMG_Load(std::string_view filename);
private:
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface;
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

    SDL_Texture* operator->();
    operator SDL_Texture* ();

    static SDL_Texture_ctx IMG_Load(SDL_Renderer_ctx& r, std::string_view filename);
private:
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
};
using TextureRef = promiscuous_ref<SDL_Texture_ctx, SDL_Texture>;
//-----------------------------------------------------------------------------
class TTF_Font_ctx {
public:
    TTF_Font_ctx(int ptsize); // use the default
    explicit TTF_Font_ctx(std::string_view filename, int ptsize);

    TTF_Font_ctx(const TTF_Font_ctx&) = delete;
    TTF_Font_ctx(TTF_Font_ctx&&) noexcept = default;
    TTF_Font_ctx& operator=(const TTF_Font_ctx&) = delete;
    TTF_Font_ctx& operator=(TTF_Font_ctx&&) noexcept = default;
    ~TTF_Font_ctx() = default;

    TTF_Font* operator->();
    operator TTF_Font* ();
private:
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font;
};
//-----------------------------------------------------------------------------
class MIX_Chunk_ctx {
public:
    MIX_Chunk_ctx(); // no sound loaded
    explicit MIX_Chunk_ctx(std::string_view filename);

    MIX_Chunk_ctx(const MIX_Chunk_ctx&) = delete;
    MIX_Chunk_ctx(MIX_Chunk_ctx&&) noexcept = default;
    MIX_Chunk_ctx& operator=(const MIX_Chunk_ctx&) = delete;
    MIX_Chunk_ctx& operator=(MIX_Chunk_ctx&&) noexcept = default;
    ~MIX_Chunk_ctx() = default;

    int PlayChannel(int channel, int loops);

    Mix_Chunk* operator->();
    operator Mix_Chunk* ();
private:
    std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> music;
};
using ChunkRef = promiscuous_ref<MIX_Chunk_ctx, Mix_Chunk>;
//-----------------------------------------------------------------------------
