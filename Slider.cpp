#include "Slider.h"
#include <SDL_image.h>

#include <iostream>

Slider::Slider(SDL_Renderer_ctx& r, int x, int y, int Width, int Height, int minvalue, int maxvalue, int value, std::function<void()> onSliderValueChanged)
    : Slider(r, x, y, Width, Height, top_left, minvalue, maxvalue, value, onSliderValueChanged)
{}

Slider::Slider(SDL_Renderer_ctx& r, int x, int y, int Width, int Height, Anchor anchor, int minvalue, int maxvalue, int value, std::function<void()> onSliderValueChanged)
    : InputDrawable(anchor), renderer(r), Marker(SDL_Texture_ctx::IMG_Load(renderer, "Drawable/Slider/Circle.png"))
{
	//Initialize all variables
	ChangeValues(minvalue, maxvalue, value);
	ChangePosition(x, y, Width, Height);

	this->onSliderValueChanged = onSliderValueChanged;
}

void Slider::SetActive(bool state) {
	InputDrawable::SetActive(state);

	if (state) {
		SDL_SetTextureColorMod(Marker, 255, 255, 255);
	} else {
		bmousepressed = false;
		SDL_SetTextureColorMod(Marker, 100, 100, 100);
	}
}

void Slider::pDraw(){
	//Drawing the slider
	//Draw the slider's background
	SDL_SetRenderDrawColor(renderer, 48, 48, 48, 255);
	SDL_RenderFillRect(renderer, &bg_rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//Draw the slider's marker
	SDL_RenderCopy(renderer, Marker, nullptr, &marker_rect);
}

void Slider::HandleInput(const SDL_Event& ev){
	if (IsActive()) {
		//Check if the user is handling the slider
		if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT && bmousepressed == false) {
			if (ev.button.x >= marker_rect.x && ev.button.x <= marker_rect.x + marker_rect.w && ev.button.y > marker_rect.y && ev.button.y < marker_rect.h + marker_rect.y) {
				bmousepressed = true;
			}
		}
		else if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT && bmousepressed == true) {
			bmousepressed = false;
		}

		//If the user is handling the slider....
		if (bmousepressed == true && marker_rect.x != ev.button.x) {
			//Move the slider's marker to the appropriate position
			if (ev.button.x > bg_rect.x + bg_rect.w - marker_rect.h + marker_rect.w / 2) {
				marker_rect.x = bg_rect.x + bg_rect.w - marker_rect.h;
			} else if (ev.button.x < bg_rect.x + marker_rect.w / 2) {
				marker_rect.x = bg_rect.x;
			} else {
				marker_rect.x = ev.button.x - marker_rect.w / 2;
			}

			//Calculate the value of the slider
			if (bg_rect.w != 0) {
				Values.Value = int((marker_rect.x - bg_rect.x + 0.0) / (bg_rect.w - marker_rect.w) * Values.Maximum);
				callOnValueChanged();
			}
		}
	}
}

void Slider::ChangeValues(int minvalue, int maxvalue, int value){
	Values = { .Value = value != -1 ? value : (minvalue + maxvalue) / 2, .Minimum = minvalue, .Maximum = maxvalue };
	callOnValueChanged();
}

void Slider::ChangePosition(int x, int y, int Width, int Height){
	marker_rect.w = marker_rect.h = Height;
	marker_rect.x = x + (Width - Height) * Values.Value / Values.Maximum;
	marker_rect.y = y;

	bg_rect.w = Width;
	bg_rect.h = Height / 5;
	bg_rect.x = x;
	bg_rect.y = y + (Height - bg_rect.h) / 2;

	ApplyAnchor(bg_rect, dAnchor);
	ApplyAnchor(marker_rect, dAnchor);
}

void Slider::callOnValueChanged(){
	if (onSliderValueChanged) {
		onSliderValueChanged();
	}
}
