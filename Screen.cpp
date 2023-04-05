#include "Screen.h"

Screen::Screen(SDL_Renderer* r){
	this->renderer = r;
}

Screen::~Screen(){
	//Delete all items created by the screen in order to avoid memory leaks
	for (int x = 0; x < InputDrawableArrtop; x++) {
		delete InputDrawableArr[x];
	}

	for (int x = 0; x < LabelArrtop; x++) {
		delete LabelArr[x];
	}

	for (int x = 0; x < ImageArrtop; x++) {
		delete ImageArr[x];
	}

	//Free all the allocated memory
	SDL_DestroyTexture(texture);
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
	for (int x = 0; x < ImageArrtop; x++) {
		ImageArr[x]->Draw();
	}

	//Calls the render method for every active button
	for (int x = 0; x < InputDrawableArrtop; x++) {
		InputDrawableArr[x]->Draw();
	}

	//Calls the render method for every active label
	for (int x = 0; x < LabelArrtop; x++) {
		LabelArr[x]->Draw();
	}
}

void Screen::Handle_Input(SDL_Event* ev){
	for (int x = 0; x < InputDrawableArrtop; x++) {
		InputDrawableArr[x]->HandleInput(ev);
	}
}

void Screen::SetupBg(const char* bg) {
	SDL_Surface* img = IMG_Load(bg);
	texture = SDL_CreateTextureFromSurface(renderer, img);
	SDL_FreeSurface(img);
}