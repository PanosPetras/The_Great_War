#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#pragma once
#include "ui/Drawable.h"

#include "core/MainWindow.h"
#include "core/SDL_ctx.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <array>
#include <functional>
#include <string>

class ToggleButton : public InputDrawable {
public:
    // Constructor
    ToggleButton(MainWindow& mw, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, std::function<void(bool)> f = {}, int keybind = 0);
    ToggleButton(MainWindow& mw, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, bool val, std::function<void(bool)> f = {}, int keybind = 0);
    ToggleButton(MainWindow& mw, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, Anchor anchor, std::function<void(bool)> f = {}, int keybind = 0);
    ToggleButton(MainWindow& mw, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, Anchor anchor, bool val, std::function<void(bool)> f = {}, int keybind = 0);

    // Destructor
    ~ToggleButton() override;

    // Called when received input, to check whether the click was in this button
    void HandleInput(const SDL_Event& ev) override;

    // Change the image assigned to the button
    void ChangeImage(std::string image, std::string inactiveImage);

    // Change the button's position
    void ChangePosition(int x, int y, int Width, int Height);

    // Change the function bound to this button
    void ChangeFunctionBinding(std::function<void(bool)> f);

    // Change the keyboard key that is bound to this button
    void ChangeKeybind(int keybind);

    // Change the current value of the button
    void ChangeValue(bool val);

    // Get the value of the toggle button
    bool GetValue();

    // Play the button's onClick sound
    void Playsound();

    // Execute the function that was bound to this button
    void CallBoundFunction();

    // Enable/Disable the button
    void SetActive(bool state) override;

protected:
    // Reference the the main window
    MainWindow* main_window;

    // Render the button on the screen
    void pDraw() override;

    // Executed when the button is clicked
    void Click();

    // The state of the button
    bool value;

    // The visual state of the button
    bool bHovered;

    // Stores the bound function
    std::function<void(bool)> func;

    // Saves the index of the keyboard key this button is bound to
    int key;

    // Dimensions of the button
    SDL_Rect draw_rect;

    /*The two images the button toggles between, each in the three visual
    states. They come out of the window's texture cache, so two checkboxes
    drawn from the same pair of files share them rather than each loading and
    uploading the files again.*/
    std::array<TextureRef, 3> activeTextures;
    std::array<TextureRef, 3> inactiveTextures;

    // Which of the three states the button currently draws itself in
    InteractiveTexture textureState = textureIdle;

    // The button's onClick sound
    SoundRef music = nullptr;
};

#endif
