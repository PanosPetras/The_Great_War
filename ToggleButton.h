#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#pragma once
#include <SDL.h>
#include <string>
#include <functional>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Drawable.h"

class ToggleButton : public InputDrawable {
public:
    //Constructor
    ToggleButton(SDL_Renderer* r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, std::function<void(bool)> f = {}, int keybind = 0);
    ToggleButton(SDL_Renderer* r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, bool val, std::function<void(bool)> f = {}, int keybind = 0);
    ToggleButton(SDL_Renderer* r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, Anchor anchor, std::function<void(bool)> f = {}, int keybind = 0);
    ToggleButton(SDL_Renderer* r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, Anchor anchor, bool val, std::function<void(bool)> f = {}, int keybind = 0);

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
    void SetActive(bool state);

protected:
    //Render the button on the screen
    void pDraw();

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
    SDL_Renderer* RendererReference;

    //Reference to the button's texture
    SDL_Texture* activeTexture = nullptr;
    SDL_Texture* inactiveTexture = nullptr;

    //The button's onClick sound
    Mix_Chunk* music = nullptr;
};

#endif
