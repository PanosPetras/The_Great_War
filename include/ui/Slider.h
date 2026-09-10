#pragma once

#include "ui/Drawable.h"

#include "core/SDL_ctx.h"

#include <SDL3/SDL.h>

#include <array>
#include <functional>

class MainWindow;

class Slider : public InputDrawable {
public:
    // Constructor, initializes the values
    Slider(MainWindow& mw, int x, int y, int Width, int Height, int minvalue = 0, int maxvalue = 100, int value = -1, std::function<void()> onSliderValueChanged = {});
    Slider(MainWindow& mw, int x, int y, int Width, int Height, Anchor anchor, int minvalue = 0, int maxvalue = 100, int value = -1, std::function<void()> onSliderValueChanged = {});

    // Handle input events
    void HandleInput(const SDL_Event& ev) override;

    // Change the slider's maximum and minimum values
    void ChangeValues(int minvalue, int maxvalue, int value = -1);

    // Change the slider's position on the screen
    void ChangePosition(int x, int y, int Width, int Height);

    // Holds the slider's values
    struct {
        int Value;
        int Minimum;
        int Maximum;
    } Values;

    // Enable/Disable the slider
    void SetActive(bool state) override;

protected:
    // A reference to the main window
    MainWindow* main_window;

    // Render the slider on the screen
    void pDraw() override;

    // Calls the function bound to this slider, when the Value changes
    void callOnValueChanged();

    /*The Slider's graphical components. The marker's three states come out of
    the window's texture cache, so every slider in the game shares them.*/
    std::array<TextureRef, 3> markerTextures;

    // Which of the three states the marker currently draws itself in
    InteractiveTexture textureState = textureIdle;

    // The rectangles of the marker and the slider
    SDL_Rect bg_rect;
    SDL_Rect marker_rect;

    // Whether the mouse has been pressed over the Marker
    bool bmousepressed = false;

private:
    // Called when the value of the slider changes
    std::function<void()> onSliderValueChanged;
};
