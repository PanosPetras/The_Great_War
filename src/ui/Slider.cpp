#include "ui/Slider.h"

#include "core/MainWindow.h"

#include <SDL3_image/SDL_image.h>

Slider::Slider(MainWindow& mw, int x, int y, int Width, int Height, int minvalue, int maxvalue, int value, std::function<void()> OnSliderValueChanged) :
    Slider(mw, x, y, Width, Height, top_left, minvalue, maxvalue, value, OnSliderValueChanged) {}

Slider::Slider(MainWindow& mw, int x, int y, int Width, int Height, Anchor anchor, int minvalue, int maxvalue, int value, std::function<void()> OnSliderValueChanged) :
    InputDrawable(anchor), main_window(&mw), markerTextures(LoadInteractiveTextures(mw, "Drawable/Slider/Circle.png")) {
    // Initialize all variables
    ChangeValues(minvalue, maxvalue, value);
    ChangePosition(x, y, Width, Height);
    onSliderValueChanged = OnSliderValueChanged;
}

void Slider::SetActive(bool state) {
    InputDrawable::SetActive(state);

    if(state) {
        textureState = textureIdle;
    } else {
        bmousepressed = false;
        textureState = textureInactive;
    }
}

void Slider::pDraw() {
    // Drawing the slider
    // Draw the slider's background
    SDL_SetRenderDrawColor(*main_window, 48, 48, 48, 255);
    const SDL_FRect bg_frect = ToFRect(bg_rect);
    SDL_RenderFillRect(*main_window, &bg_frect);
    SDL_SetRenderDrawColor(*main_window, 0, 0, 0, 255);

    // Draw the slider's marker
    RenderTexture(*main_window, markerTextures[textureState], marker_rect);
}

void Slider::HandleInput(const SDL_Event& ev) {
    if(!IsActive()) return;

    /*Nothing here can act on an event that carries no cursor position, and
    reading one out of an event that has none - a key press, or a wheel event,
    whose union member holds a scroll amount at that offset - would drag the
    marker off to wherever those bytes happened to land.*/
    float mx, my;
    if(!GetMousePosition(ev, mx, my)) return;

    // Check if the user is handling the slider
    if(ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN && ev.button.button == SDL_BUTTON_LEFT && bmousepressed == false) {
        if(IsPointInRect(marker_rect, mx, my)) {
            bmousepressed = true;
        }
    } else if(ev.type == SDL_EVENT_MOUSE_BUTTON_UP && ev.button.button == SDL_BUTTON_LEFT && bmousepressed == true) {
        bmousepressed = false;
    }

    // If the user is handling the slider....
    if(bmousepressed == true && float(marker_rect.x) != mx) {
        // Move the slider's marker to the appropriate position
        if(mx > float(bg_rect.x + bg_rect.w - marker_rect.h + marker_rect.w / 2)) {
            marker_rect.x = bg_rect.x + bg_rect.w - marker_rect.h;
        } else if(mx < float(bg_rect.x + marker_rect.w / 2)) {
            marker_rect.x = bg_rect.x;
        } else {
            marker_rect.x = int(mx) - marker_rect.w / 2;
        }

        // Calculate the value of the slider
        if(bg_rect.w != 0) {
            Values.Value = int((marker_rect.x - bg_rect.x + 0.0) / (bg_rect.w - marker_rect.w) * Values.Maximum);
            callOnValueChanged();
        }
    }
}

void Slider::ChangeValues(int minvalue, int maxvalue, int value) {
    Values = {.Value = value != -1 ? value : (minvalue + maxvalue) / 2, .Minimum = minvalue, .Maximum = maxvalue};
    callOnValueChanged();
}

void Slider::ChangePosition(int x, int y, int Width, int Height) {
    marker_rect.w = marker_rect.h = Height;
    marker_rect.x = x + (Width - Height) * Values.Value / Values.Maximum;
    marker_rect.y = y;

    bg_rect.w = Width;
    bg_rect.h = Height / 5;
    bg_rect.x = x;
    bg_rect.y = y + (Height - bg_rect.h) / 2;

    ApplyAnchor(bg_rect, dAnchor);
    ApplyAnchor(marker_rect, dAnchor);
}

void Slider::callOnValueChanged() {
    if(onSliderValueChanged) {
        onSliderValueChanged();
    }
}
