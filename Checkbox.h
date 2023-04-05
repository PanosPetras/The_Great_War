#ifndef CHECKBOX_H
#define CHECKBOX_H

#pragma once
#include <SDL.h>
#include <string>
#include "ToggleButton.h"
#include "Label.h"
#include <functional>

class Checkbox : public Drawable {
public:
	//Constructor
	Checkbox(SDL_Renderer* r, int x, int y, int Height, std::string text, int textSize, std::function<void(bool)> f = NULL, int keybind = NULL);
	Checkbox(SDL_Renderer* r, int x, int y, int Height, std::string text, int textSize, Anchor anchor, std::function<void(bool)> f = NULL, int keybind = NULL);

	//Destructor
	~Checkbox();

	//Called when received input, to check whether the click was in this button
	void HandleInput(const SDL_Event* ev);

	//Change the button's position
	void ChangePosition(int x, int y, int Height);

	//Change the function bound to this button
	void ChangeFunctionBinding(std::function<void(bool)> f);

	//Change the keyboard key that is bound to this button
	void ChangeKeybind(int keybind);

    //Change the Checkboxes text
	void ChangeText(std::string text);

	//Change the Checkboxes text size
	void ChangeTextSize(int size);

	//Change the current value of the button
	void ChangeValue(bool val);

	//Get the value of the toggle button
	bool GetValue();

	//Play the button's onClick sound
	void Playsound();

	//Execute the function that was bound to this button
	void CallBoundFunction();

protected:
	//Render the Checkbox on the screen
	void pDraw();

	//The Drawables that make up this Drawable
	ToggleButton* toggleButton;
	Label* label;
};

#endif
