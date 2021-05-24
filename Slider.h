#ifndef Slider_H
#define Slider_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Slider {
public:
	//Constructor, initializes the values
	Slider(SDL_Renderer* r, int x, int y, int Width, int Height, int minvalue = 0, int maxvalue = 100);

	//Destructor, frees up the memory
	~Slider();

	//Render the slider on the screen
	void RenderSlider();

	//Handle input events
	int HandleInput(SDL_Event* ev);

	//Change the slider's maximum and minimum values
	void ChangeValues(int minvalue, int maxvalue);

	//Change the slider's position on the screen
	void ChangePosition(int x, int y);

	//Change the slider's size
	void ChangeSize(int Width, int Height);

	//The Slider's graphical components
	SDL_Texture* Background;
	SDL_Texture* Marker;

	SDL_Rect bg_rect;
	SDL_Rect marker_rect;

	//A reference to the window's renderer
	SDL_Renderer* renderer;

	//Whether the mouse has been pressed over the Marker
	bool bmousepressed;

	//Holds the slider's values
	struct {
		int Value;
		int Minimum;
		int Maximum;
	} Values;
};
#endif