#include "Button.h"
#include <SDL_ttf.h>

Button::Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string image, std::function<void()> f, int keybind) : Button(r, x, y, Width, Height, image, top_left, f, keybind) {
}

Button::Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string image, std::function<void(void*)> f, void* arg, int keybind) : Button(r, x, y, Width, Height, image, top_left, f, arg, keybind) {
}

Button::Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string image, Anchor anchor, std::function<void()> f, int keybind) : InputDrawable(anchor) {
    //Saving the renderer's reference
    RendererReference = r;

    //Sets the default value for the button's state
    bHovered = false;

    //Saving the button's coordinates
    ChangePosition(x, y, Width, Height);

    //Loading the button texture
    ChangeImage(image);

    //Saving the bound function
    ChangeFunctionBinding(f);

    //Save the button's bind to the keyboard
    ChangeKeybind(keybind);

    //Load the button's click sound
    music = Mix_LoadWAV("Sounds/ButtonClick.mp3");
}

Button::Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string image, Anchor anchor, std::function<void(void*)> f, void* arg, int keybind) : Button(r, x, y, Width, Height, image, anchor, NULL, keybind) {
    //Saving the bound function
    ChangeFunctionBinding(f, arg);
}

Button::Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string text, int textSize, std::function<void()> f, int keybind) : Button(r, x, y, Width, Height, text, textSize, top_left, f, keybind) {
}

Button::Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string text, int textSize, std::function<void(void*)> f, void* arg, int keybind) : Button(r, x, y, Width, Height, text, textSize, top_left, f, arg, keybind) {
}

Button::Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string text, int textSize, Anchor anchor, std::function<void()> f, int keybind) : InputDrawable(anchor) {
    //Saving the renderer's reference
    RendererReference = r;

    //Sets the default value for the button's state
    bHovered = false;

    //Saving the button's coordinates
    ChangePosition(x, y, Width, Height);

    //Load the Button's Texture and add text on top of it
    ChangeText(text, textSize);

    //Saving the bound function
    ChangeFunctionBinding(f);

    //Save the button's bind to the keyboard
    ChangeKeybind(keybind);

    //Load the button's click sound
    music = Mix_LoadWAV("Sounds/ButtonClick.mp3");
}

Button::Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string text, int textSize, Anchor anchor, std::function<void(void*)> f, void* arg, int keybind) : Button(r, x, y, Width, Height, text, textSize, anchor) {
    //Saving the bound function
    ChangeFunctionBinding(f, arg);
}

Button::~Button() {
    //Free up the memory
    SDL_DestroyTexture(texture);

    if (text != nullptr) {
        SDL_DestroyTexture(text);
    }

    Mix_FreeChunk(music);
}

void Button::pDraw() {
    //Drawing the button
    SDL_RenderCopy(RendererReference, texture, NULL, &draw_rect);

    if (text != nullptr) {
        //Add text on top of Button background

        SDL_RenderCopy(RendererReference, text, NULL, &text_draw_rect);
    }
}

void Button::HandleInput(const SDL_Event* ev) {
    //Detect if the mouse is hovered
    if (ev->button.x >= this->draw_rect.x &&
        ev->button.x <= (this->draw_rect.x + this->draw_rect.w) &&
        ev->button.y >= this->draw_rect.y &&
        ev->button.y <= (this->draw_rect.y + this->draw_rect.h)) {

        if (bHovered == false) {
            //If the button is hovered, change to the hovered button image
            SDL_SetTextureColorMod(texture, 170, 170, 170);
            SDL_SetTextureColorMod(text, 170, 170, 170);
            bHovered = true;
        }

        //react on mouse click within button rectangle
        if (ev->type == SDL_MOUSEBUTTONDOWN) {
            Click();
        }
    }
    //If not hovered, return to the idle button image
    else if (bHovered == true) {
        bHovered = false;
        SDL_SetTextureColorMod(texture, 255, 255, 255);
        SDL_SetTextureColorMod(text, 255, 255, 255);
    }
    if (key) {
        if (ev->type == SDL_KEYDOWN && ev->key.keysym.sym == key) {
            Click();
        }
    }
}

void Button::Click(){
    //Play the sound effect
    Playsound();

    //Execute the bound function, if it was assigned on the creation of the button
    CallBoundFunction();
}

void Button::ChangeImage(std::string image) {
    //Saving the new image path
    auto imagePath = image + ".png";

    //Destroy the texture if it already exists
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
    if (this->text != nullptr) {
        SDL_DestroyTexture(this->text);
    }

    //Load the button texture
    SDL_Surface* img = IMG_Load(imagePath.c_str());
    texture = SDL_CreateTextureFromSurface(RendererReference, img);
    SDL_FreeSurface(img);
}

void Button::ChangeText(std::string text, int textSize){
    //Destroy the texture if it already exists
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
    if (this->text != nullptr) {
        SDL_DestroyTexture(this->text);
        this->text = nullptr;
    }

    //Load the button background
    SDL_Surface* bg = IMG_Load("Drawable/Button/Button.png");
    texture = SDL_CreateTextureFromSurface(RendererReference, bg);

    //Free the background surface
    SDL_FreeSurface(bg);

    //Loading the font from the file
    TTF_Font* font = TTF_OpenFont(GLOBAL_FONT, textSize);

    //Convert the text to a surface
    SDL_Surface* textSur = TTF_RenderText_Blended(font, text.c_str(), SDL_Color{.r = 255, .g = 255, .b = 255});
    this->text = SDL_CreateTextureFromSurface(RendererReference, textSur);

    int text_x = (draw_rect.w - textSur->w) / 2;
    int text_y = (draw_rect.h - textSur->h) / 3;
    text_draw_rect = {  .x = draw_rect.x + (text_x > 0 ? text_x : int(draw_rect.w * 0.1)),
                        .y = draw_rect.y + text_y,
                        .w = (text_x > 0 ? textSur->w : int(draw_rect.w * 0.8)),
                        .h = textSur->h };

    ChangePosition(draw_rect.x, draw_rect.y, draw_rect.w, draw_rect.h);

    //Free the surface
    SDL_FreeSurface(textSur);

    //Delete the font
    TTF_CloseFont(font);
}

void Button::ChangePosition(int x, int y, int Width, int Height) {
    //Saving the new button's coordinates
    draw_rect.x = x;
    draw_rect.y = y;
    draw_rect.w = Width;
    draw_rect.h = Height;

    switch (dAnchor) {
        case top_left:
            break;
        case top_right:
            draw_rect.x -= Width;
            break;
        case bottom_left:
            draw_rect.y -= Height;
            break;
        case bottom_right:
            draw_rect.x -= Width;
            draw_rect.y -= Height;
            break;
        case center:
            draw_rect.x -= Width / 2;
            draw_rect.y -= Height / 2;
            break;
    }
}

void Button::ChangeFunctionBinding(std::function<void()> f) {
    //saving the newly bound function
    funcWArg = NULL;
    this->arg = NULL;
    func = f;
}

void Button::ChangeFunctionBinding(std::function<void(void*)> f, void* arg){
    //saving the newly bound function
    funcWArg = f;
    this->arg = arg;
    func = NULL;
}

void Button::ChangeKeybind(int keybind){
    key = keybind;
}

void Button::Playsound() {
    Mix_PlayChannel(1, music, 0);
}

void Button::CallBoundFunction(){
    if (func) {
        func();
    } else if (funcWArg) {
        funcWArg(arg);
    }
}