#include "Screen.h"
#include "MessageBox.h"
#include "Label.h"
#include "Image.h"

#include <iostream>

Screen::Screen(SDL_Renderer* r) : renderer(r) {
        std::cerr << "Screen::Screen()\t" << static_cast<void*>(this) << std::endl;
}

Screen::Screen(SDL_Renderer* r, std::function<void()> qf, std::function<void(std::unique_ptr<Screen>)> csf) : renderer(r), QuitFunc(qf), ChangeScreenFunc(csf) {
        std::cerr << "Screen::Screen(...)\t" << static_cast<void*>(this) << std::endl;
}

Screen::~Screen(){
	//Free all the allocated memory
	if (bHasBackground) {
		SDL_DestroyTexture(texture);
	}
        std::cerr << "Screen::~Screen\t" << static_cast<void*>(this) << std::endl;
}

void Screen::RenderBackground() {
	/*Check if the rendered Image must be zoomed.
	If it musn't, then we just cope the image to the surface.
	If it does, then we create a rectangle and give it the
	appropriate dimensions, based on the magnification
	factor reiceived from user input*/
	if (bHasBackground) {
            SDL_RenderCopy(renderer, texture, NULL, NULL);
        }
}

void Screen::Render(){
	//Calls the method responsible for rendering the background
        RenderBackground();

	//Calls the render method for every active image
	for (auto& image : ImageArr) {
		image->Draw();
	}

	//Calls the render method for every active button
		for(auto& drawable : InputDrawableArr) {
		drawable->Draw();
	}

	//Calls the render method for every active label
	for (auto& label : LabelArr) {
		label->Draw();
	}

	for (auto& msgb : messageBoxes) {
		msgb->Draw();
	}
}

void Screen::Handle_Input(SDL_Event& ev){
	for(auto& drawable : InputDrawableArr) {
                drawable->HandleInput(ev);
	}

	for (auto& msgb : messageBoxes) {
		msgb->HandleInput(ev);
	}
}

void Screen::SetupBg(const char* bg) {
    if(bHasBackground) {
        SDL_DestroyTexture(texture);
    } else bHasBackground = true;

    SDL_Surface* img = IMG_Load(bg);
    texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
}

void Screen::DeleteMessageBox(void* p) {
	MessageBox* msgb = static_cast<MessageBox*>(p);

	auto res = std::remove_if(messageBoxes.begin(), messageBoxes.end(), [msgb](auto& ptr) { return ptr.get() == msgb; });
	messageBoxes.erase(res, messageBoxes.end());
}
