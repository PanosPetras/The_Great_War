#pragma once

#include "Drawable.h"

#include "SDL_ctx.h"

#include <SDL.h>

#include <functional>

class MainWindow;

class Slider : public InputDrawable {
public:
    // Constructor, initializes the values
    Slider(MainWindow& mw, int x, int y, int Width, int Height, int minvalue = 0, int maxvalue = 100, int value = -1,
           std::function<void()> onSliderValueChanged = {});
    Slider(MainWindow& mw, int x, int y, int Width, int Height, Anchor anchor, int minvalue = 0, int maxvalue = 100, int value = -1,
           std::function<void()> onSliderValueChanged = {});
    ~Slider() override;

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

    // The Slider's graphical components
    SDL_Texture_ctx Marker;

    // The rectangles of the marker and the slider
    SDL_Rect bg_rect;
    SDL_Rect marker_rect;

    // Whether the mouse has been pressed over the Marker
    bool bmousepressed = false;

private:
    // Called when the value of the slider changes
    std::function<void()> onSliderValueChanged;
};
