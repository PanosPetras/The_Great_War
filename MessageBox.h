#pragma once

#include "Button.h"
#include "Drawable.h"
#include "Image.h"
#include "Label.h"

#include <SDL.h>

#include <functional>
#include <memory>
#include <string>

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
