#include "Button.h"

Button::Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string image, std::function<void()> f, int keybind) {
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

Button::Button(SDL_Renderer* r, int x, int y, int Width, int Height, std::string image, std::function<void(void*)> f, void* arg, int keybind) : Button(r, x, y, Width, Height, image, NULL, keybind) {
    //Saving the bound function
    ChangeFunctionBinding(f, arg);
}

Button::~Button() {
    //Free up the memory
    SDL_DestroyTexture(texture);
    Mix_FreeChunk(music);
}

void Button::Draw() {
    //Drawing the button
    SDL_RenderCopy(RendererReference, texture, NULL, &draw_rect);
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

    //Load the button texture
    SDL_Surface* img = IMG_Load(imagePath.c_str());
    texture = SDL_CreateTextureFromSurface(RendererReference, img);
    SDL_FreeSurface(img);
}

void Button::ChangePosition(int x, int y, int Width, int Height) {
    //Saving the new button's coordinates
    this->draw_rect.x = x;
    this->draw_rect.y = y;
    this->draw_rect.w = Width;
    this->draw_rect.h = Height;
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
