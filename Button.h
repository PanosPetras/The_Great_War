#ifndef BUTTON_H
#define BUTTON_H

#pragma once
#include <SDL.h>
#include <string>
#include <functional>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Drawable.h"

class Button : public InputDrawable {
public:
    //Constructors
    Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string image, std::function<void()> f = nullptr, int keybind = 0);
    Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string image, std::function<void(void*)> f, void* arg, int keybind = 0);
    Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string image, Anchor anchor, std::function<void()> f = nullptr, int keybind = 0);
    Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string image, Anchor anchor, std::function<void(void*)> f, void* arg, int keybind = 0);
    Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string text, int textSize, std::function<void()> f = nullptr, int keybind = 0);
    Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string text, int textSize, std::function<void(void*)> f, void* arg, int keybind = 0);
    Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string text, int textSize, Anchor anchor, std::function<void()> f = nullptr, int keybind = 0);
    Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string text, int textSize, Anchor anchor, std::function<void(void*)> f, void* arg, int keybind = 0);

    //Destructor
    ~Button();

    //Called when received input, to check whether the click was in this button
    void HandleInput(const SDL_Event& ev) override;

    //Change the image assigned to the button
    void ChangeImage(std::string image);

    //Change the text assigned to the button
    void ChangeText(std::string text, int textSize);

    //Change the button's position
    void ChangePosition(int x, int y, int Width, int Height);

    //Change the function bound to this button
    void ChangeFunctionBinding(std::function<void()> f);
    void ChangeFunctionBinding(std::function<void(void*)> f, void* arg);

    //Change the keyboard key that is bound to this button
    void ChangeKeybind(int keybind);

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

    //Check if the mouse is within the limits of an SDL_Rect
    static bool CheckIfMouseInRect(const SDL_Rect& rect, const SDL_MouseButtonEvent& ev);

    //The visual state of the button
    bool bHovered;

    //Stores the bound function
    std::function<void()> func;
    std::function<void(void*)> funcWArg;
    void* arg = nullptr;

    //Saves the index of the keyboard key this button is bound to
    int key;

    //Dimensions of the button
    SDL_Rect draw_rect;
    SDL_Rect text_draw_rect;

    //Reference the the screen's renderer
    SDL_Renderer* RendererReference;

    //Reference to the button's texture
    SDL_Texture* texture = nullptr;
    SDL_Texture* text = nullptr;

    //The button's onClick sound
    Mix_Chunk* music = nullptr;

    friend class ToggleButton;
};
#endif
