#ifndef BUTTON_H
#define BUTTON_H

#pragma once
#include <string>
#include <functional>
#include "Drawable.h"
#include "Image.h"
#include "Label.h"

typedef struct SDL_Renderer SDL_Renderer;

class TextEntry : public InputDrawable {
public:
    //Constructor
    TextEntry(SDL_Renderer* r, int x, int y, int Width, int Height, std::string defaultText = "");
    TextEntry(SDL_Renderer* r, int x, int y, int Width, int Height, std::string defaultText, int maxCharacters);
    TextEntry(SDL_Renderer* r, int x, int y, int Width, int Height, std::string defaultText, std::string hint = "");
    TextEntry(SDL_Renderer* r, int x, int y, int Width, int Height, std::string defaultText, std::string hint, int maxCharacters);

    //Destructor
    ~TextEntry();

    //Called when received input, to check whether the click was in this button
    void HandleInput(const SDL_Event* ev);

    //Change the button's position
    void ChangePosition(int x, int y, int Width, int Height);

    bool IsFocused();

    std::string GetText();

    void ChangeText(std::string text);

    std::string GetHint();

    void ChangeHint(std::string hint);

protected:
    //Render the button on the screen
    void pDraw();

    //The position of the Entry
    int x, y;

    //The text of the Entry
    std::string text;
    std::string hint;

    //The maximum size of the Entry's text
    int maxSize;

    //The visual state of the button
    bool focused;

    //Reference the the screen's renderer
    SDL_Renderer* RendererReference;

    //The components of the Entry
    Image* background;
    Label* textLabel, *hintLabel;
};
#endif