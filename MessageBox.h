#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#pragma once
#include <SDL.h>
#include <string>
#include <functional>
#include <memory>
#include "Button.h"
#include "Drawable.h"

class Image;
class Label;

class MessageBox : public InputDrawable {
public:
	//Constructor
	MessageBox(SDL_Renderer_ctx& r, std::string title, std::string message, std::function<void(void*)> f);

	//Called when received input
	void HandleInput(const SDL_Event& ev) override;

private:
	//Render the message box on screen
	void pDraw();

	std::unique_ptr<Image> background;
	std::unique_ptr<Label> title;
	std::unique_ptr<Label> text;
	std::unique_ptr<Button> okButton;
};

#endif
