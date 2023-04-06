#include "ToggleButton.h"
#include <iostream>
#include "Button.h"

ToggleButton::ToggleButton(SDL_Renderer* r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, std::function<void(bool)> f, int keybind) : ToggleButton(r, x, y, Width, Height, activeImage, inactiveImage, top_left, f, keybind) {
}

ToggleButton::ToggleButton(SDL_Renderer* r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, bool val, std::function<void(bool)> f, int keybind) : ToggleButton(r, x, y, Width, Height, activeImage, inactiveImage, top_left, val, f, keybind) {
}

ToggleButton::ToggleButton(SDL_Renderer* r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, Anchor anchor, std::function<void(bool)> f, int keybind) : InputDrawable(anchor) {
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

ToggleButton::ToggleButton(SDL_Renderer* r, int x, int y, int Width, int Height, std::string activeImage, std::string inactiveImage, Anchor anchor, bool val, std::function<void(bool)> f, int keybind) {
    value = val;
}

ToggleButton::~ToggleButton(){
    //Free up the memory
    SDL_DestroyTexture(activeTexture);
    SDL_DestroyTexture(inactiveTexture);
    Mix_FreeChunk(music);
}

void ToggleButton::pDraw(){
    //Drawing the toggle button
    if (!value) {
        SDL_RenderCopy(RendererReference, inactiveTexture, NULL, &draw_rect);
    } else {
        SDL_RenderCopy(RendererReference, activeTexture, NULL, &draw_rect);
    }
}

void ToggleButton::HandleInput(const SDL_Event* ev){
    if (active) {
        //Detect if the button is hovered
        if (Button::CheckIfMouseInRect(draw_rect, ev->button)) {

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

void ToggleButton::ChangePosition(int x, int y, int Width, int Height){
    //Saving the new button's coordinates
    draw_rect = { .x = x, .y = y, .w = Width, .h = Height };

    ApplyAnchor(draw_rect, dAnchor);
}

void ToggleButton::ChangeFunctionBinding(std::function<void(bool)> f){
    func = f;
}

void ToggleButton::ChangeKeybind(int keybind){
    key = keybind;
}

void ToggleButton::ChangeValue(bool val){
    value = val;
}

void ToggleButton::Playsound(){
    Mix_PlayChannel(1, music, 0);
}

void ToggleButton::CallBoundFunction(){
	if (func) {
		func(value);
	}
}

void ToggleButton::SetActive(bool state) {
    InputDrawable::SetActive(state);

    if (state) {
        SDL_SetTextureColorMod(inactiveTexture, 255, 255, 255);
        SDL_SetTextureColorMod(activeTexture, 255, 255, 255);
        bHovered = false;
    }
    else {
        SDL_SetTextureColorMod(inactiveTexture, 100, 100, 100);
        SDL_SetTextureColorMod(activeTexture, 100, 100, 100);
    }

}

bool ToggleButton::GetValue() {
    return value;
}
