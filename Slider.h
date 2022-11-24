#ifndef Slider_H
#define Slider_H

#pragma once
#include <SDL.h>
#include <functional>
#include "Drawable.h"

class Slider : public InputDrawable {
public:
	//Constructor, initializes the values
	Slider(SDL_Renderer* r, int x, int y, int Width, int Height, int minvalue = 0, int maxvalue = 100, int value = -1, std::function<void()> onSliderValueChanged = NULL);

	//Destructor, frees up the memory
	~Slider();

	//Handle input events
	void HandleInput(const SDL_Event* ev);

	//Change the slider's maximum and minimum values
	void ChangeValues(int minvalue, int maxvalue, int value = -1);

	//Change the slider's position on the screen
	void ChangePosition(int x, int y);

	//Change the slider's size
	void ChangeSize(int Width, int Height);

	//Called when the value of the slider changes
	std::function<void()> onSliderValueChanged;

	//Holds the slider's values
	struct {
		int Value;
		int Minimum;
		int Maximum;
	} Values;

protected:
	//Render the slider on the screen
	void pDraw();

	//Calls the function bound to this slider, when the Value changes
	void callOnValueChanged();

	//A reference to the window's renderer
	SDL_Renderer* renderer;

	//The Slider's graphical components
	SDL_Texture* Marker;

	//The rectangles of the marker and the slider
	SDL_Rect bg_rect;
	SDL_Rect marker_rect;

	//Whether the mouse has been pressed over the Marker
	bool bmousepressed;
};
#endif