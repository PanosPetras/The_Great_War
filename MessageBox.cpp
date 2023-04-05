#include "MessageBox.h"

MessageBox::MessageBox(SDL_Renderer* r, std::string message) {
	background = new Image(r, "bg", 0, 0, 0, 0);
	okButton = new Button(r, 0, 0, 0, 0, "Ok", 32);
}

MessageBox::~MessageBox() {
	delete background, okButton;
}

void MessageBox::HandleInput(const SDL_Event* ev) {
	okButton->HandleInput(ev);
}

void MessageBox::pDraw() {
	background->Draw();
	okButton->Draw();
}
