#include "Button.h"
#include "MainWindow.h"

#include <SDL_ttf.h>

#include <iostream>
#include <unordered_set>

namespace {
    std::array<TextureRef, 3> Load(MainWindow& mw, std::string image) {
        std::cerr << "Button::Load " << image << std::endl;
        return mw.IMG_Load(image + ".png",
            [](SDL_Texture_ctx& texture){ // inactive
                SDL_SetTextureColorMod(texture, 100, 100, 100);
            },
            [](SDL_Texture_ctx& texture){ // idle
                SDL_SetTextureColorMod(texture, 255, 255, 255);
            },
            [](SDL_Texture_ctx& texture) { // hoovered
                SDL_SetTextureColorMod(texture, 170, 170, 170);
            }
        );
    }
}

std::unordered_set<Button*> deads; // Only used for debugging

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string image, std::function<void()> f, int keybind) :
    Button(mw, x, y, Width, Height, image, top_left, f, keybind) {
}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string image, std::function<void(void*)> f, void* arg, int keybind) :
    Button(mw, x, y, Width, Height, image, top_left, f, arg, keybind) {
}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string image, Anchor anchor, std::function<void()> f, int keybind) :
    InputDrawable(anchor),
    main_window(&mw),
    textures{Load(mw, image)}
{
    deads.erase(this); // if a previous Button had the same address, remove it from the dead Buttons
    std::cerr << "Button::Button image: " << image << std::endl;

    //Saving the button's coordinates
    ChangePosition(x, y, Width, Height);

    //Loading the button texture
    //ChangeImage(image);

    //Saving the bound function
    ChangeFunctionBinding(f);

    //Save the button's bind to the keyboard
    ChangeKeybind(keybind);

    //Load the button's click sound
    music = Mix_LoadWAV("Sounds/ButtonClick.mp3");
}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string image, Anchor anchor, std::function<void(void*)> f, void* arg, int keybind) :
    Button(mw, x, y, Width, Height, image, anchor, nullptr, keybind) {
    //Saving the bound function
    ChangeFunctionBinding(f, arg);
}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string text, int textSize, std::function<void()> f, int keybind) : Button(mw, x, y, Width, Height, text, textSize, top_left, f, keybind) {
}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string text, int textSize, std::function<void(void*)> f, void* arg, int keybind) :
    Button(mw, x, y, Width, Height, text, textSize, top_left, f, arg, keybind)
{
}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string text, int textSize, Anchor anchor, std::function<void()> f, int keybind) :
    Button(mw, x, y, Width, Height, "Drawable/Button/Button", anchor, f, keybind)
{
    deads.erase(this); // if a previous Button had the same address, remove it from the dead Buttons
    std::cerr << "Button::Button text: " << text << std::endl;

    //Saving the button's coordinates
    ChangePosition(x, y, Width, Height);

    //Load the Button's Texture and add text on top of it
    ChangeText(text, textSize);

    //Saving the bound function
    ChangeFunctionBinding(f);

    //Save the button's bind to the keyboard
    ChangeKeybind(keybind);
}

Button::Button(MainWindow& mw, int x, int y, int Width, int Height, std::string text, int textSize, Anchor anchor, std::function<void(void*)> f, void* arg, [[maybe_unused]] int keybind) :
    Button(mw, x, y, Width, Height, text, textSize, anchor)
{
    //Saving the bound function
    ChangeFunctionBinding(f, arg);
}

void log(Button* This, const char* txt) {
    if(deads.contains(This))
        std::cerr << txt << " (on DEAD)\t" << static_cast<void*>(This) << std::endl;
    else
        std::cerr << txt << " (on live)\t" << static_cast<void*>(This) << std::endl;
}

Button::~Button() {
    std::cerr << "Button::~Button\t" << static_cast<void*>(this) << std::endl;
    deads.emplace(this);

    Mix_FreeChunk(music);
}

void Button::pDraw() {
    //Drawing the button
    SDL_RenderCopy(*main_window, active_texture, nullptr, &draw_rect);

    if (text != nullptr) {
        //Add text on top of Button background
        SDL_RenderCopy(*main_window, text, nullptr, &text_draw_rect);
    }
}

void Button::HandleInput(const SDL_Event& ev) {
    if (active) {
        //Detect if the button is hovered
        if (CheckIfMouseInRect(draw_rect, ev.button)) {
            if (bHovered == false) {
                //If the button is hovered, change to the hovered button image
                active_texture = textures[textureHoovered];
                SDL_SetTextureColorMod(text, 170, 170, 170);
                bHovered = true;
            }

            //react on mouse click within button rectangle
            if (ev.type == SDL_MOUSEBUTTONDOWN) {
                Click();
            }
        }
        //If not hovered, return to the idle button image
        else if (bHovered == true) {
            bHovered = false;
            active_texture = textures[textureIdle];
            SDL_SetTextureColorMod(text, 255, 255, 255);
        }

        if (key) {
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == key) {
                Click();
            }
        }
    } else {
        active_texture = textures[textureInactive];
    }
}

void Button::Click(){
    log(this, "Button::Click");
    //Play the sound effect
    Playsound();

    //Execute the bound function, if it was assigned on the creation of the button
    CallBoundFunction();
}

bool Button::CheckIfMouseInRect(const SDL_Rect& rect, const SDL_MouseButtonEvent& ev) {
    return (ev.x >= rect.x) && (ev.x <= rect.x + rect.w) && (ev.y >= rect.y) && (ev.y <= rect.y + rect.h);
}

void Button::ChangeText(std::string textstr, int textSize){
    //Loading the font from the file
    TTF_Font_ctx font(textSize);

    //Convert the text to a surface
    SDL_Surface_ctx textSur(TTF_RenderText_Blended(font, textstr.c_str(), SDL_Color{.r = 255, .g = 255, .b = 255, .a = 0}));
    text = SDL_Texture_ctx(*main_window, textSur);

    int text_x = (draw_rect.w - textSur->w) / 2;
    int text_y = (draw_rect.h - textSur->h) / 3;
    text_draw_rect = {  .x = draw_rect.x + (text_x > 0 ? text_x : int(draw_rect.w * 0.1)),
                        .y = draw_rect.y + text_y,
                        .w = (text_x > 0 ? textSur->w : int(draw_rect.w * 0.8)),
                        .h = textSur->h };

    bHovered = false;
}

void Button::ChangePosition(int x, int y, int Width, int Height) {
    //Saving the new button's coordinates
    draw_rect = { .x = x, .y = y, .w = Width, .h = Height };

    ApplyAnchor(draw_rect, dAnchor);
}

void Button::ChangeFunctionBinding(std::function<void()> f) {
    //saving the newly bound function
    func = f;
}

void Button::ChangeFunctionBinding(std::function<void(void*)> f, void* arg){
    //saving the newly bound function
    func = [f=std::move(f), arg]{ f(arg); };
}

void Button::ChangeKeybind(int keybind){
    key = keybind;
}

void Button::Playsound() {
    Mix_PlayChannel(1, music, 0);
}

void Button::CallBoundFunction(){
    log(this, "Button::CallBoundFunction 1");
    if (func) {
        MainWindow::Instance().AddEvent([func=func]{ func(); });
        log(this, "Button::CallBoundFunction added func MainWindow event list");
    }
}

void Button::SetActive(bool state) {
    InputDrawable::SetActive(state);

    if (state) {
        active_texture = textures[textureIdle];
        SDL_SetTextureColorMod(text, 255, 255, 255);
        bHovered = false;
    } else {
        active_texture = textures[textureInactive];
        SDL_SetTextureColorMod(text, 100, 100, 100);
    }
}
