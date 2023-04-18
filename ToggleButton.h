#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#pragma once
#include "Drawable.h"

#include "SDL_ctx.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <string>
#include <functional>

class ToggleButton : public InputDrawable {
public:
    //Constructor
    ToggleButton(SDL_Renderer_ctx& r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, std::function<void(bool)> f = {}, int keybind = 0);
    ToggleButton(SDL_Renderer_ctx& r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, bool val, std::function<void(bool)> f = {}, int keybind = 0);
    ToggleButton(SDL_Renderer_ctx& r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, Anchor anchor, std::function<void(bool)> f = {}, int keybind = 0);
    ToggleButton(SDL_Renderer_ctx& r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, Anchor anchor, bool val, std::function<void(bool)> f = {}, int keybind = 0);

    //Destructor
    ~ToggleButton();

    //Called when received input, to check whether the click was in this button
    void HandleInput(const SDL_Event& ev) override;

    //Change the image assigned to the button
    void ChangeImage(std::string image, std::string inactiveImage);

    //Change the button's position
    void ChangePosition(int x, int y, int Width, int Height);

    //Change the function bound to this button
    void ChangeFunctionBinding(std::function<void(bool)> f);

    //Change the keyboard key that is bound to this button
    void ChangeKeybind(int keybind);

    //Change the current value of the button
    void ChangeValue(bool val);

    //Get the value of the toggle button
    bool GetValue();

    //Play the button's onClick sound
    void Playsound();

    //Execute the function that was bound to this button
    void CallBoundFunction();

    //Enable/Disable the button
    void SetActive(bool state) override;

protected:
    //Render the button on the screen
    void pDraw() override;

    //Executed when the button is clicked
    void Click();

    //The state of the button
    bool value;

    //The visual state of the button
    bool bHovered;

    //Stores the bound function
    std::function<void(bool)> func;

    //Saves the index of the keyboard key this button is bound to
    int key;

    //Dimensions of the button
    SDL_Rect draw_rect;

    //Reference the the screen's renderer
    RendererRef RendererReference;

    //Reference to the button's texture
    SDL_Texture_ctx activeTexture;
    SDL_Texture_ctx inactiveTexture;

    //The button's onClick sound
    Mix_Chunk* music = nullptr;
};

#endif
