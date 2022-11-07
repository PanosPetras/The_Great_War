#include "Button.h"

Button::Button(SDL_Renderer* r, int x, int y, int Height, int Width, const char* image, std::function<void()> f, int keybind) {
    //music = nullptr;
    music = Mix_LoadWAV("Sounds/ButtonClick.wav");

    //Saving the button's coordinates
    this->draw_rect.x = x;
    this->draw_rect.y = y;
    this->draw_rect.w = Height;
    this->draw_rect.h = Width;

    //Saving the renderer's reference
    RendererReference = r;

    //Saving the image path
    imagepath = image;
    CurrentImage = imagepath + ".png";

    //saving the binded function
    func = f;

    //Loading the button texture
    SDL_Surface* img = IMG_Load(CurrentImage.c_str());
    texture = SDL_CreateTextureFromSurface(RendererReference, img);
    SDL_FreeSurface(img);

    //Sets the default value for the button's state
    bHovered = false;
    
    if (keybind) {
        key = keybind;
    } else {
        key = NULL;
    }
}

Button::~Button() {
    //Free up the memory
    SDL_DestroyTexture(texture);
    Mix_FreeChunk(music);
}

void Button::button_process_event(const SDL_Event* ev) {
    //Detect if the mouse is hovered
    if (ev->button.x >= this->draw_rect.x &&
        ev->button.x <= (this->draw_rect.x + this->draw_rect.w) &&
        ev->button.y >= this->draw_rect.y &&
        ev->button.y <= (this->draw_rect.y + this->draw_rect.h)) {

        if (bHovered == false) {
            //If the button is hovered, change to the hovered button image
            SDL_SetTextureColorMod(texture, 170, 170, 170);
            //CurrentImage = imagepath + "_Hovered.png";
            bHovered = true;
            //Update();
        }

        //react on mouse click within button rectangle
        if (ev->type == SDL_MOUSEBUTTONDOWN) {
            //Play the sound effect
            Playsound();

            //Execute the binded function, if it was assigned on the creation of the button
            if (func) {
                func();
            }
        }
    }
    //If not hovered, return to the idle button image
    else if (bHovered == true) {
        bHovered = false;
        SDL_SetTextureColorMod(texture, 255, 255, 255);
    }
    if (key) {
        if (ev->type == SDL_KEYDOWN && ev->key.keysym.sym == key) {
            //Play the sound effect
            Playsound();

            //Execute the binded function, if it was assigned on the creation of the button
            if (func) {
                func();
            }
        }
    }
}

void Button::RenderButton() {
    //Drawing the button
    SDL_RenderCopy(RendererReference, texture, NULL, &draw_rect);
}

void Button::ChangeImage(const char* image) {
    //Saving the new image path
    imagepath = image;
    CurrentImage = imagepath + ".png";
    Update();
}

void Button::ChangePosition(int x, int y, int Height, int Width) {
    //Saving the new button's coordinates
    this->draw_rect.x = x;
    this->draw_rect.y = y;
    this->draw_rect.w = Height;
    this->draw_rect.h = Width;
}

void Button::ChangeFunctionBinding(std::function<void()> f) {
    //saving the newly binded function
    func = f;
}

void Button::Update(){
    //Free up the memory
    SDL_DestroyTexture(texture);

    //Loading the button texture
    SDL_Surface* img = IMG_Load(CurrentImage.c_str());
    texture = SDL_CreateTextureFromSurface(RendererReference, img);
    SDL_FreeSurface(img);
}

void Button::Playsound() {
    Mix_PlayChannel(1, music, 0);
}