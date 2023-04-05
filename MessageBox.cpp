#include "MessageBox.h"
#include "WindowInfo.h"

MessageBox::MessageBox(SDL_Renderer* r, std::string title, std::string message) {
	int Width = GetWindowWidth(), Height = GetWindowHeight();

	background = new Image(r, "bg", int(Width * 0.4), int(Height * 0.4), int(Width * 0.2), int(Height * 0.2));
	this->title = new Label(r, title, 42, 0, 0);
	text = new Label(r, message, 32, 0, 0);
	okButton = new Button(r, 0, 0, 0, 0, "Ok", 32);
}

MessageBox::~MessageBox() {
	delete background, okButton, text;
}

void MessageBox::HandleInput(const SDL_Event* ev) {
	okButton->HandleInput(ev);
}

void MessageBox::pDraw() {
	background->Draw();
	okButton->Draw();
}
