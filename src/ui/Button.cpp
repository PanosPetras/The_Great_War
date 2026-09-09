#include "ui/Button.h"
#include "core/MainWindow.h"

#include <SDL3_ttf/SDL_ttf.h>

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string image, std::function<void()> f, int keybind) : Button(mw, x, y, Width, Height, image, top_left, f, keybind) {}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string image, std::function<void(void*)> f, void* arg, int keybind) : Button(mw, x, y, Width, Height, image, top_left, f, arg, keybind) {}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string image, Anchor anchor, std::function<void()> f, int keybind) :
    InputDrawable(anchor), main_window(&mw), textures{LoadInteractiveTextures(mw, image + ".png")}, music{mw.LoadSound("Sounds/ButtonClick.wav")} {
    // Saving the button's coordinates
    ChangePosition(x, y, Width, Height);

    // Saving the bound function
    ChangeFunctionBinding(f);

    // Save the button's bind to the keyboard
    ChangeKeybind(keybind);
}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string image, Anchor anchor, std::function<void(void*)> f, void* arg, int keybind) : Button(mw, x, y, Width, Height, image, anchor, nullptr, keybind) {
    // Saving the bound function
    ChangeFunctionBinding(f, arg);
}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string Text, FontSize textSize, std::function<void()> f, int keybind) : Button(mw, x, y, Width, Height, Text, textSize, top_left, f, keybind) {}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string Text, FontSize textSize, std::function<void(void*)> f, void* arg, int keybind) :
    Button(mw, x, y, Width, Height, Text, textSize, top_left, f, arg, keybind) {}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string Text, FontSize textSize, Anchor anchor, std::function<void()> f, int keybind) :
    Button(mw, x, y, Width, Height, "Drawable/Button/Button", anchor, f, keybind) {
    // Saving the button's coordinates
    ChangePosition(x, y, Width, Height);

    // Load the Button's Texture and add text on top of it
    ChangeText(Text, textSize);

    // Saving the bound function
    ChangeFunctionBinding(f);

    // Save the button's bind to the keyboard
    ChangeKeybind(keybind);
}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string Text, FontSize textSize, Anchor anchor, std::function<void(void*)> f, void* arg, [[maybe_unused]] int keybind) :
    Button(mw, x, y, Width, Height, Text, textSize, anchor) {
    // Saving the bound function
    ChangeFunctionBinding(f, arg);
}

void Button::pDraw() {
    // Drawing the button
    RenderTexture(*main_window, active_texture, draw_rect);

    if(text != nullptr) {
        // Add text on top of Button background
        RenderTexture(*main_window, text, text_draw_rect);
    }
}

void Button::HandleInput(const SDL_Event& ev) {
    if(!IsActive()) {
        active_texture = textures[textureInactive];
        return;
    }

    /*Only an event that carries a cursor position can say anything about what
    is hovered - asking a key press where the mouse is reads a coordinate out
    of a part of the union that is holding something else entirely.*/
    if(float mx, my; GetMousePosition(ev, mx, my)) {
        // Detect if the button is hovered
        if(IsPointInRect(draw_rect, mx, my)) {
            if(bHovered == false) {
                // If the button is hovered, change to the hovered button image
                active_texture = textures[textureHoovered];
                SetTextTint(170);
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
            active_texture = textures[textureIdle];
            SetTextTint(255);
        }
    }

    if(key && ev.type == SDL_EVENT_KEY_DOWN && ev.key.key == static_cast<SDL_Keycode>(key)) {
        Click();
    }
}

void Button::Click() {
    // Play the sound effect
    Playsound();

    // Execute the bound function, if it was assigned on the creation of the button
    CallBoundFunction();
}

void Button::SetTextTint(Uint8 level) {
    // A button built from an image alone has no text texture to tint
    if(text) SDL_SetTextureColorMod(text, level, level, level);
}

void Button::ChangeText(std::string textstr, FontSize textSize) {
    // The window keeps every font open, so this does not touch the disk
    FontRef font = main_window->TTF_OpenFont(textSize);

    // Convert the text to a surface
    auto textSur = SDL_Surface_ctx::TTF_RenderText_Blended(font, textstr, SDL_Color{.r = 255, .g = 255, .b = 255, .a = 0});
    text = SDL_Texture_ctx(*main_window, textSur);

    int text_x = (draw_rect.w - textSur->w) / 2;
    int text_y = (draw_rect.h - textSur->h) / 3;
    text_draw_rect = {.x = draw_rect.x + (text_x > 0 ? text_x : int(draw_rect.w * 0.1)), .y = draw_rect.y + text_y, .w = (text_x > 0 ? textSur->w : int(draw_rect.w * 0.8)), .h = textSur->h};

    bHovered = false;
}

void Button::ChangePosition(int x, int y, int Width, int Height) {
    // Saving the new button's coordinates
    draw_rect = {.x = x, .y = y, .w = Width, .h = Height};

    ApplyAnchor(draw_rect, dAnchor);
}

void Button::ChangeFunctionBinding(std::function<void()> f) {
    // saving the newly bound function
    func = f;
}

void Button::ChangeFunctionBinding(std::function<void(void*)> f, void* arg) {
    // saving the newly bound function
    func = [f = std::move(f), arg] { f(arg); };
}

void Button::ChangeKeybind(int keybind) {
    key = keybind;
}

void Button::Playsound() {
    if(music) music->Play();
}

void Button::CallBoundFunction() {
    if(func) {
        main_window->AddEvent([func = func] { func(); });
    }
}

void Button::SetActive(bool state) {
    InputDrawable::SetActive(state);

    if(state) {
        active_texture = textures[textureIdle];
        SetTextTint(255);
        bHovered = false;
    } else {
        active_texture = textures[textureInactive];
        SetTextTint(100);
    }
}
