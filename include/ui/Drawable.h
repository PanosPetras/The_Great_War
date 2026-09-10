#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "core/SDL_ctx.h"

#include <SDL3/SDL.h>

#include <array>
#include <string>

class MainWindow;

enum Anchor { top_left, top_right, bottom_left, bottom_right, center, center_top, center_bottom, center_left, center_right };

class Drawable {
public:
    Drawable(Anchor anchor = top_left);
    virtual ~Drawable() = default;

    void Draw();

    bool Visibility;

protected:
    Anchor dAnchor;

    virtual void pDraw() = 0;
};

class InputDrawable : public Drawable {
public:
    InputDrawable(Anchor anchor = top_left);

    virtual void HandleInput(const SDL_Event&) = 0;
    virtual void SetActive(bool state);

    bool IsActive();

private:
    bool active;
};

void ApplyAnchor(SDL_Rect& rect, Anchor anchor);

/*Reads the cursor position out of an event, returning false when the event is
not one that carries a position. Only the mouse events do: an SDL_Event is a
union, so reading ev.button.x out of, say, a key press yields whatever that
event happens to keep at that offset rather than a coordinate.*/
bool GetMousePosition(const SDL_Event& ev, float& x, float& y);

// Whether a point, usually one GetMousePosition just produced, is inside a rectangle
bool IsPointInRect(const SDL_Rect& rect, float x, float y);

// The visual states an interactive component draws itself in
enum InteractiveTexture { textureIdle, textureHoovered, textureInactive };

/*The three tinted copies of an image that an interactive component draws
itself with: untouched when idle, dimmed when hovered, dimmer still when
disabled. Switching between them costs nothing at draw time, which is why
components hold all three rather than re-tinting one texture as their state
changes - a texture is shared out of the window's cache, so re-tinting one
would recolour every other component drawn from the same file.*/
std::array<TextureRef, 3> LoadInteractiveTextures(MainWindow& mw, const std::string& filename);

#endif
