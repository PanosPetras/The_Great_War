#ifndef SCREEN_H
#define SCREEN_H

#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Button.h"
#include "Label.h"
#include "Image.h"
#include "Slider.h"

class Screen{
public:
	//Constructor
	Screen(SDL_Renderer* r, int Width, int Height);

	//Destructor
	virtual ~Screen();

	//This is a reference to the main window's renderer
	SDL_Renderer* renderer;

	//Allows the creation of up to 60 Images on the screen
	Image* ImageArr[60];

	//This is the count of the Images being displayed
	int ImageArrtop;

	//Allows the creation of up to 25 Buttons on the screen
	Button* ButtonArr[25];

	//This is the count of the Buttons being displayed
	int ButtonArrtop;

	//Allows the creation of up to 35 Labels on the screen
	Label* LabelArr[35];

	//This is the count of the Labels being displayed
	int LabelArrtop;

	//Allows the creation of up to 35 Labels on the screen
	Slider* SliderArr[5];

	//This is the count of the Labels being displayed
	int SliderArrtop;

	//Stores the main window's dimensions
	int WindowSize[2];
	
	bool bHasBackground;

	//Sets the screen's background
	virtual void RenderBackground();

	//Renders all of the screen's components
	virtual void Render();
	
	//Handles input events
	virtual void Handle_Input(SDL_Event* ev);
	
	void SetupBg(const char* bg);

	//Stores the image's texture
	SDL_Texture* texture;

	std::function<void(Screen*)> ChangeScreenFunc;
	std::function<void()> QuitFunc;
};
#endif