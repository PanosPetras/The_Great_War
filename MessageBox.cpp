#include "MessageBox.h"
#include "WindowInfo.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"

MessageBox::MessageBox(SDL_Renderer* r, std::string title, std::string message, std::function<void(void*)> f) {
	int Width = GetWindowWidth(), Height = GetWindowHeight();

	background = new Image(r, "Backgrounds/old_paper.png", int(Width * 0.3), int(Height * 0.3), int(Width * 0.4), int(Height * 0.4));
	this->title = new Label(r, title, 42, int(Width * 0.5), int(Height * 0.34), int(Width * 0.35), center);
	text = new Label(r, message, 32, int(Width * 0.5), int(Height * 0.37), int(Width * 0.35), center_top);
	okButton = new Button(r, int(Width * 0.67), int(Height * 0.66), int(Width * 0.09), int(Height * 0.055), "OK", 26, bottom_right, f, this, SDLK_RETURN);
}

MessageBox::~MessageBox() {
	delete background;
        delete okButton;
        delete text;
        delete title;
}

void MessageBox::HandleInput(const SDL_Event* ev) {
	okButton->HandleInput(ev);
}

void MessageBox::pDraw() {
	background->Draw();
	title->Draw();
	text->Draw();
	okButton->Draw();
}
