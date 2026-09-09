#include "ui/Drawable.h"

#include "core/MainWindow.h"

#include <SDL3/SDL.h>

Drawable::Drawable(Anchor anchor) {
    Visibility = true;
    dAnchor = anchor;
}

void Drawable::Draw() {
    if(Visibility) {
        pDraw();
    }
}

InputDrawable::InputDrawable(Anchor anchor) : Drawable(anchor) {
    active = true;
}

void InputDrawable::SetActive(bool state) {
    active = state;
}

bool InputDrawable::IsActive() {
    return active;
}

void ApplyAnchor(SDL_Rect& rect, Anchor anchor) {
    switch(anchor) {
    case top_left:
        break;
    case top_right:
        rect.x -= rect.w;
        break;
    case bottom_left:
        rect.y -= rect.h;
        break;
    case bottom_right:
        rect.x -= rect.w;
        rect.y -= rect.h;
        break;
    case center:
        rect.x -= rect.w / 2;
        rect.y -= rect.h / 2;
        break;
    case center_top:
        rect.x -= rect.w / 2;
        break;
    case center_bottom:
        rect.x -= rect.w / 2;
        rect.y -= rect.h;
        break;
    case center_left:
        rect.y -= rect.h / 2;
        break;
    case center_right:
        rect.x -= rect.w;
        rect.y -= rect.h / 2;
        break;
    }
}

bool GetMousePosition(const SDL_Event& ev, float& x, float& y) {
    switch(ev.type) {
    case SDL_EVENT_MOUSE_MOTION:
        x = ev.motion.x;
        y = ev.motion.y;
        return true;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        x = ev.button.x;
        y = ev.button.y;
        return true;
    case SDL_EVENT_MOUSE_WHEEL:
        x = ev.wheel.mouse_x;
        y = ev.wheel.mouse_y;
        return true;
    default:
        return false;
    }
}

bool IsPointInRect(const SDL_Rect& rect, float x, float y) {
    return x >= float(rect.x) && x <= float(rect.x + rect.w) && y >= float(rect.y) && y <= float(rect.y + rect.h);
}

std::array<TextureRef, 3> LoadInteractiveTextures(MainWindow& mw, const std::string& filename) {
    return mw.IMG_Load(
        filename,
        [](SDL_Texture_ctx& texture) { // idle
            SDL_SetTextureColorMod(texture, 255, 255, 255);
        },
        [](SDL_Texture_ctx& texture) { // hoovered
            SDL_SetTextureColorMod(texture, 170, 170, 170);
        },
        [](SDL_Texture_ctx& texture) { // inactive
            SDL_SetTextureColorMod(texture, 100, 100, 100);
        });
}
