#include "Screen.h"
#include "MessageBox.h"
#include "Label.h"
#include "Image.h"

#include <iostream>

Screen::Screen(SDL_Renderer* r){
	renderer = r;
}

Screen::~Screen(){
	//Delete all items created by the screen in order to avoid memory leaks
	for (auto msgb : messageBoxes) {
		delete msgb;
	}

	//Free all the allocated memory
	if (bHasBackground) {
		SDL_DestroyTexture(texture);
	}
}

void Screen::RenderBackground() {
	/*Check if the rendered Image must be zoomed.
	If it musn't, then we just cope the image to the surface.
	If it does, then we create a rectangle and give it the
	appropriate dimensions, based on the magnification
	factor reiceived from user input*/
	SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void Screen::Render(){
	//Calls the method responsible for rendering the background
	if (bHasBackground) {
		this->RenderBackground();
	}

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

	for (auto msgb : messageBoxes) {
		msgb->Draw();
	}
}

void Screen::Handle_Input(SDL_Event* ev){
	for(auto& drawable : InputDrawableArr) {
		if (drawable) {
			drawable->HandleInput(ev);
		} else {
			std::cerr << "invalid drawable\n";
		}
	}

	for (auto msgb : messageBoxes) {
		msgb->HandleInput(ev);
	}
}

void Screen::SetupBg(const char* bg) {
	SDL_Surface* img = IMG_Load(bg);
	texture = SDL_CreateTextureFromSurface(renderer, img);
	SDL_FreeSurface(img);
}

void Screen::DeleteMessageBox(void* p) {
	MessageBox* msgb = (MessageBox*)p;

	auto res = remove(messageBoxes.begin(), messageBoxes.end(), msgb);
	messageBoxes.erase(res, messageBoxes.end());

	delete msgb;
}
