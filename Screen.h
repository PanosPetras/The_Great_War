#ifndef SCREEN_H
#define SCREEN_H

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <functional>

class MessageBox;
class InputDrawable;
class Image;
class Label;

class Screen{
public:
	//Constructor
	Screen(SDL_Renderer* r);

	//Destructor
	virtual ~Screen();

	//Renders all of the screen's components
	virtual void Render();

	//Handles input events
	virtual void Handle_Input(SDL_Event* ev);

protected:
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

	std::vector<MessageBox*> messageBoxes;
	
	bool bHasBackground;

	//Sets the screen's background
	virtual void RenderBackground();
	
	void SetupBg(const char* bg);

	void DeleteMessageBox(void* p);

	//Stores the image's texture
	SDL_Texture* texture;

	std::function<void(Screen*)> ChangeScreenFunc;
	std::function<void()> QuitFunc;
};
#endif