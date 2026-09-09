#include "screens/Screen.h"

#include "ui/Button.h"
#include "ui/Drawable.h"
#include "ui/Image.h"
#include "ui/Label.h"
#include "core/MainWindow.h"
#include "ui/MessageBox.h"

#include <algorithm>
#include <iostream>

Screen::Screen(MainWindow& mw) : main_window(&mw) {
    std::cerr << "Screen::Screen()\t" << static_cast<void*>(this) << '\n';
}

Screen::Screen(MainWindow& mw, std::function<void()> qf, std::function<void(std::unique_ptr<Screen>)> csf) : main_window(&mw), QuitFunc(qf), ChangeScreenFunc(csf) {
    std::cerr << "Screen::Screen(...)\t" << static_cast<void*>(this) << '\n';
}

void Screen::Update(Uint32) {
    // Most screens are static and have nothing to advance
}

void Screen::RenderBackground() {
    /*Check if the rendered Image must be zoomed.
    If it musn't, then we just cope the image to the surface.
    If it does, then we create a rectangle and give it the
    appropriate dimensions, based on the magnification
    factor reiceived from user input*/
    if(texture.valid()) {
        SDL_RenderTexture(*main_window, texture, nullptr, nullptr);
    }
}

void Screen::Render() {
    // Calls the method responsible for rendering the background
    RenderBackground();

    // Calls the render method for every active image
    for(auto& image : ImageArr) {
        image->Draw();
    }

    // Calls the render method for every active button
    for(auto& drawable : InputDrawableArr) {
        drawable->Draw();
    }

    // Calls the render method for every active label
    for(auto& label : LabelArr) {
        label->Draw();
    }

    for(auto& msgb : messageBoxes) {
        msgb->Draw();
    }
}

void Screen::Handle_Input(SDL_Event& ev) {
    for(auto& drawable : InputDrawableArr) {
        drawable->HandleInput(ev);
    }

    for(auto& msgb : messageBoxes) {
        msgb->HandleInput(ev);
    }
}

void Screen::SetupBg(const char* bg) {
    bHasBackground = true;
    texture = main_window->IMG_Load(bg);
}

void Screen::DeleteMessageBox(void* p) {
    MessageBox* msgb = static_cast<MessageBox*>(p);

    auto res = std::remove_if(messageBoxes.begin(), messageBoxes.end(), [msgb](auto& ptr) { return ptr.get() == msgb; });
    messageBoxes.erase(res, messageBoxes.end());
}
