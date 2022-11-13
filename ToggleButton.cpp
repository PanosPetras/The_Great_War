#include "ToggleButton.h"
#include <iostream>

ToggleButton::ToggleButton(SDL_Renderer* r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, std::function<void(bool)> f, int keybind){
    //Saving the renderer's reference
    RendererReference = r;

    //Set the default values
    bHovered = false;
    value = false;

    //Saving the button's coordinates
    ChangePosition(x, y, Width, Height);

    //Loading the button textures
    ChangeImage(activeImage, inactiveImage);

    //Saving the bound function
    ChangeFunctionBinding(f);

    //Save the button's bind to the keyboard
    ChangeKeybind(keybind);

    //Load the button's click sound
    music = Mix_LoadWAV("Sounds/ButtonClick.mp3");
}

ToggleButton::~ToggleButton(){
    //Free up the memory
    SDL_DestroyTexture(activeTexture);
    SDL_DestroyTexture(inactiveTexture);
    Mix_FreeChunk(music);
}

void ToggleButton::Draw(){
    //Drawing the toggle button
    if (!value) {
        SDL_RenderCopy(RendererReference, inactiveTexture, NULL, &draw_rect);
    } else {
        SDL_RenderCopy(RendererReference, activeTexture, NULL, &draw_rect);
    }
}

void ToggleButton::HandleInput(const SDL_Event* ev){
    //Detect if the mouse is hovered
    if (ev->button.x >= this->draw_rect.x &&
        ev->button.x <= (this->draw_rect.x + this->draw_rect.w) &&
        ev->button.y >= this->draw_rect.y &&
        ev->button.y <= (this->draw_rect.y + this->draw_rect.h)) {

        if (bHovered == false) {
            //If the button is hovered, change to the hovered button image
            SDL_SetTextureColorMod(inactiveTexture, 170, 170, 170);
            SDL_SetTextureColorMod(activeTexture, 170, 170, 170);
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
        SDL_SetTextureColorMod(inactiveTexture, 255, 255, 255);
        SDL_SetTextureColorMod(activeTexture, 255, 255, 255);
    }
    if (key) {
        if (ev->type == SDL_KEYDOWN && ev->key.keysym.sym == key) {
            Click();
        }
    }
}

void ToggleButton::Click() {
    //Change the value of the button
    value = !value;

    //Play the sound effect
    Playsound();

    //Execute the bound function, if it was assigned on the creation of the button
    CallBoundFunction();
}

void ToggleButton::ChangeImage(std::string activeImage, std::string inactiveImage){
    //Saving the image path
    auto activeImagePath = activeImage + ".png";
    auto inactiveImagePath = inactiveImage + ".png";

    //Destroy the textures, if they already exist
    if (activeTexture != nullptr) {
        SDL_DestroyTexture(activeTexture);
    }
    if (inactiveTexture != nullptr) {
        SDL_DestroyTexture(inactiveTexture);
    }

    //Loading the button's textures
    SDL_Surface* img = IMG_Load(activeImagePath.c_str());
    activeTexture = SDL_CreateTextureFromSurface(RendererReference, img);
    SDL_FreeSurface(img);

    img = IMG_Load(inactiveImagePath.c_str());
    inactiveTexture = SDL_CreateTextureFromSurface(RendererReference, img);
    SDL_FreeSurface(img);
}

void ToggleButton::ChangePosition(int x, int y, int Height, int Width){
    //Saving the new button's coordinates
    this->draw_rect.x = x;
    this->draw_rect.y = y;
    this->draw_rect.w = Width;
    this->draw_rect.h = Height;
}

void ToggleButton::ChangeFunctionBinding(std::function<void(bool)> f){
    func = f;
}

void ToggleButton::ChangeKeybind(int keybind){
    key = keybind;
}

void ToggleButton::Playsound(){
    Mix_PlayChannel(1, music, 0);
}

void ToggleButton::CallBoundFunction(){
	if (func) {
		func(value);
	}
}

bool ToggleButton::GetValue() {
    return value;
}
