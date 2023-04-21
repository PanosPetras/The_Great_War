#ifndef BUTTON_H
#define BUTTON_H

#pragma once
#include "Drawable.h"

#include <functional>
#include <memory>
#include <string>

class Image;
class Label;
class MainWindow;

class TextEntry : public InputDrawable {
public:
    // Constructor
    TextEntry(MainWindow& mw, int x, int y, int Width, int Height, std::string defaultText, int maxCharacters = 30);
    TextEntry(MainWindow& mw, int x, int y, int Width, int Height, std::string defaultText, std::string hint = "",
              int maxCharacters = 30);
    TextEntry(MainWindow& mw, int x, int y, int Width, int Height, Anchor anchor, std::string defaultText, int maxCharacters = 30);
    TextEntry(MainWindow& mw, int x, int y, int Width, int Height, Anchor anchor, std::string defaultText, std::string hint = "",
              int maxCharacters = 30);

    // Destructor
    ~TextEntry() override = default;

    // Called when received input, to check whether the click was in this button
    void HandleInput(const SDL_Event& ev) override;

    // Change the button's position
    void ChangePosition(int x, int y, int Width, int Height);

    bool IsFocused();

    std::string GetText();

    void ChangeText(std::string text);

    std::string GetHint();

    void ChangeHint(std::string hint);

protected:
    // Reference the the screen's main window
    MainWindow* main_window;

    // Render the button on the screen
    void pDraw() override;

    // The position of the Entry
    int x, y;

    // The text of the Entry
    std::string text;
    std::string hint;

    // The maximum size of the Entry's text
    int maxSize;

    // The visual state of the button
    bool focused;

    // The components of the Entry
    std::unique_ptr<Image> background;
    std::unique_ptr<Label> textLabel;
    std::unique_ptr<Label> hintLabel;
};
#endif
