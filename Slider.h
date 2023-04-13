#ifndef Slider_H
#define Slider_H

#pragma once
#include "Drawable.h"

#include "SDL_ctx.h"

#include <SDL.h>

#include <functional>

class Slider : public InputDrawable {
public:
	//Constructor, initializes the values
	Slider(SDL_Renderer_ctx& r, int x, int y, int Width, int Height, int minvalue = 0, int maxvalue = 100, int value = -1, std::function<void()> onSliderValueChanged = {});
	Slider(SDL_Renderer_ctx& r, int x, int y, int Width, int Height, Anchor anchor, int minvalue = 0, int maxvalue = 100, int value = -1, std::function<void()> onSliderValueChanged = {});

	//Destructor, frees up the memory
	~Slider();

	//Handle input events
	void HandleInput(const SDL_Event& ev) override;

	//Change the slider's maximum and minimum values
	void ChangeValues(int minvalue, int maxvalue, int value = -1);

	//Change the slider's position on the screen
	void ChangePosition(int x, int y, int Width, int Height);

	//Holds the slider's values
	struct {
		int Value;
		int Minimum;
		int Maximum;
	} Values;

	//Enable/Disable the slider
	void SetActive(bool state);

protected:
	//Render the slider on the screen
	void pDraw();

	//Calls the function bound to this slider, when the Value changes
	void callOnValueChanged();

	//A reference to the window's renderer
	RendererRef renderer;

	//The Slider's graphical components
	SDL_Texture* Marker;

	//The rectangles of the marker and the slider
	SDL_Rect bg_rect;
	SDL_Rect marker_rect;

	//Whether the mouse has been pressed over the Marker
	bool bmousepressed = false;

private:
	//Called when the value of the slider changes
	std::function<void()> onSliderValueChanged;
};
#endif
