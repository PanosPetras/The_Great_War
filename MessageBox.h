#pragma once

#include "Drawable.h"

#include <functional>
#include <memory>
#include <string>

class Button;
class Image;
class Label;
class MainWindow;

class MessageBox : public InputDrawable {
public:
    // Constructor
    MessageBox(MainWindow& mw, std::string title, std::string message, std::function<void(void*)> f);

    // Called when received input
    void HandleInput(const SDL_Event& ev) override;

private:
    // Render the message box on screen
    void pDraw() override;

    std::unique_ptr<Image> background;
    std::unique_ptr<Label> title;
    std::unique_ptr<Label> text;
    std::unique_ptr<Button> okButton;
};
