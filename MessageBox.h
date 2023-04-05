#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#pragma once
#include <SDL.h>
#include <string>
#include "Drawable.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"

class MessageBox : InputDrawable {
public:
	//Constructor
	MessageBox(SDL_Renderer* r, std::string message);

	//Destructor
	~MessageBox();

	//Called when received input
	void HandleInput(const SDL_Event* ev);

private:
	//Render the message box on screen
	void pDraw();

	Image* background;
	Label* text;
	Button* okButton;
};

#endif