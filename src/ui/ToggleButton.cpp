#include "ui/ToggleButton.h"

#include "ui/Button.h"

ToggleButton::ToggleButton(MainWindow& mw, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, std::function<void(bool)> f, int keybind) :
    ToggleButton(mw, x, y, Width, Height, activeImage, inactiveImage, top_left, f, keybind) {}

ToggleButton::ToggleButton(MainWindow& mw, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, bool val, std::function<void(bool)> f, int keybind) :
    ToggleButton(mw, x, y, Width, Height, activeImage, inactiveImage, top_left, val, f, keybind) {}

ToggleButton::ToggleButton(MainWindow& mw, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, Anchor anchor, std::function<void(bool)> f, int keybind) :
    InputDrawable(anchor), main_window(&mw), music(mw.LoadSound("Sounds/ButtonClick.wav")) {
    // Set the default values
    bHovered = false;
    value = false;

    // Saving the button's coordinates
    ChangePosition(x, y, Width, Height);

    // Loading the button textures
    ChangeImage(activeImage, inactiveImage);

    // Saving the bound function
    ChangeFunctionBinding(f);

    // Save the button's bind to the keyboard
    ChangeKeybind(keybind);
}

ToggleButton::ToggleButton(MainWindow& mw, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, Anchor anchor, bool val, std::function<void(bool)> f, int keybind) :
    ToggleButton(mw, x, y, Width, Height, std::move(activeImage), std::move(inactiveImage), anchor, std::move(f), keybind) {
    value = val;
}

/*The window owns every sound and hands out non-owning references to them, so
there is nothing here left to free.*/
ToggleButton::~ToggleButton() = default;

void ToggleButton::pDraw() {
    // Drawing the toggle button, in whichever state it is currently in
    auto& textures = value ? activeTextures : inactiveTextures;
    RenderTexture(*main_window, textures[textureState], draw_rect);
}

void ToggleButton::HandleInput(const SDL_Event& ev) {
    if(!IsActive()) return;

    /*Only an event that carries a cursor position can say anything about what
    is hovered - asking a key press where the mouse is reads a coordinate out
    of a part of the union that is holding something else entirely.*/
    if(float mx, my; GetMousePosition(ev, mx, my)) {
        // Detect if the button is hovered
        if(IsPointInRect(draw_rect, mx, my)) {
            if(bHovered == false) {
                // If the button is hovered, change to the hovered button image
                textureState = textureHoovered;
                bHovered = true;
            }

            // react on mouse click within button rectangle
            if(ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                Click();
            }
        }
        // If not hovered, return to the idle button image
        else if(bHovered == true) {
            bHovered = false;
            textureState = textureIdle;
        }
    }

    if(key && ev.type == SDL_EVENT_KEY_DOWN && ev.key.key == static_cast<SDL_Keycode>(key)) {
        Click();
    }
}

void ToggleButton::Click() {
    // Change the value of the button
    value = !value;

    // Play the sound effect
    Playsound();

    // Execute the bound function, if it was assigned on the creation of the button
    CallBoundFunction();
}

void ToggleButton::ChangeImage(std::string activeImage, std::string inactiveImage) {
    // Loading the button's textures, which the window keeps for us
    activeTextures = LoadInteractiveTextures(*main_window, activeImage + ".png");
    inactiveTextures = LoadInteractiveTextures(*main_window, inactiveImage + ".png");
}

void ToggleButton::ChangePosition(int x, int y, int Width, int Height) {
    // Saving the new button's coordinates
    draw_rect = {.x = x, .y = y, .w = Width, .h = Height};

    ApplyAnchor(draw_rect, dAnchor);
}

void ToggleButton::ChangeFunctionBinding(std::function<void(bool)> f) {
    func = f;
}

void ToggleButton::ChangeKeybind(int keybind) {
    key = keybind;
}

void ToggleButton::ChangeValue(bool val) {
    value = val;
}

void ToggleButton::Playsound() {
    if(music) music->Play();
}

void ToggleButton::CallBoundFunction() {
    if(func) {
        func(value);
    }
}

void ToggleButton::SetActive(bool state) {
    InputDrawable::SetActive(state);

    if(state) {
        textureState = textureIdle;
        bHovered = false;
    } else {
        textureState = textureInactive;
    }
}

bool ToggleButton::GetValue() {
    return value;
}
