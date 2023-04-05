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
	Screen(SDL_Renderer* r);

	//Destructor
	virtual ~Screen();

	//This is a reference to the main window's renderer
	SDL_Renderer* renderer;

	//Allows the creation of up to 60 Images on the screen
	Image* ImageArr[60];

	//This is the count of the Images being displayed
	int ImageArrtop;

	//Allows the creation of up to 70 Drawable elements on the screen
	InputDrawable* InputDrawableArr[70];

	//This is the count of the Buttons being displayed
	int InputDrawableArrtop;

	//Allows the creation of up to 35 Labels on the screen
	Label* LabelArr[35];

	//This is the count of the Labels being displayed
	int LabelArrtop;
	
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