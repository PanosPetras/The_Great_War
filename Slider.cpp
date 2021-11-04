#include "Slider.h"

Slider::Slider(SDL_Renderer* r, int x, int y, int Width, int Height, int minvalue, int maxvalue){
	//Saving a reference to the renderer
	renderer = r;
	
	//Initialize all variables
	ChangeValues(minvalue, maxvalue);
	ChangeSize(Width, Height);
	ChangePosition(x, y);

	//Load the slider's textures
	SDL_Surface* temp = IMG_Load("Slider/Marker.png");
	Marker = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	temp = IMG_Load("Slider/Background.png");
	Background = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
}

Slider::~Slider(){
	//Freeing up the memory
	SDL_DestroyTexture(Background);
	SDL_DestroyTexture(Marker);
}

void Slider::RenderSlider(){
	//Drawing the slider
	SDL_RenderCopy(renderer, Background, NULL, &bg_rect);
	SDL_RenderCopy(renderer, Marker, NULL, &marker_rect);
}

int Slider::HandleInput(SDL_Event* ev){
	if (ev->button.button == SDL_BUTTON_LEFT && ev->type == SDL_MOUSEBUTTONDOWN && bmousepressed == false) {
		if (ev->button.x >= marker_rect.x && ev->button.x <= marker_rect.x + marker_rect.w && ev->button.y > marker_rect.y && ev->button.y < marker_rect.h + marker_rect.y) {
			bmousepressed = true;
		}
	} else if (ev->button.button == SDL_BUTTON_LEFT && ev->type == SDL_MOUSEBUTTONUP && bmousepressed == true) {
		bmousepressed = false;
	}
	if (bmousepressed == true) {
		if (bg_rect.w != 0) {
			Values.Value = int((marker_rect.x - bg_rect.x - bg_rect.w * 0.09) / (bg_rect.w * 0.76) * Values.Maximum);
		}
		if (ev->button.x > bg_rect.x + bg_rect.w * 0.85) {
			if (marker_rect.x != bg_rect.x + bg_rect.w * 0.85) {
				marker_rect.x = int(bg_rect.x + bg_rect.w * 0.85);
			}
		} else if (ev->button.x < bg_rect.x + bg_rect.w * 0.09) {
			if (marker_rect.x != bg_rect.x + bg_rect.w * 0.09) {
				marker_rect.x = int(bg_rect.x + bg_rect.w * 0.09);
			}
		}
		else {
			marker_rect.x = ev->button.x;
		}
	}

	return Values.Value;
}

void Slider::ChangeValues(int minvalue, int maxvalue){
	Values = { .Value = int((minvalue + maxvalue) / 2), .Minimum = minvalue, .Maximum = maxvalue };
}

void Slider::ChangePosition(int x, int y){
	bg_rect.x = x;
	bg_rect.y = y;
	marker_rect.x = bg_rect.x + int(bg_rect.w * Values.Value / Values.Maximum - bg_rect.w * 0.08 / 2);
	marker_rect.y = bg_rect.y;
}

void Slider::ChangeSize(int Width, int Height){
	bg_rect.w = Width;
	bg_rect.h = Height;
	marker_rect.w = int(bg_rect.w * 0.08);
	marker_rect.h = bg_rect.h;
}