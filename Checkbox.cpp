#include "Checkbox.h"
#include "ToggleButton.h"
#include "Label.h"

Checkbox::Checkbox(SDL_Renderer_ctx& r, int x, int y, int Height, std::string text, int textSize, [[maybe_unused]]Anchor anchor, std::function<void(bool)> f, int keybind) :
    toggleButton(r, x, y, Height, Height, "Drawable/Checkbox/Ticked Checkbox", "Drawable/Checkbox/Empty Checkbox", f, keybind),
    label(r, text, textSize, x + int(Height * (1 + 0.1)), y)
{
}

void Checkbox::pDraw() {
	toggleButton.Draw();
	label.Draw();
}

void Checkbox::HandleInput(const SDL_Event& ev) {
	toggleButton.HandleInput(ev);
}

void Checkbox::ChangePosition(int x, int y, int Height) {
	toggleButton.ChangePosition(x, y, Height, Height);
	label.ChangePosition(x, y);
}

void Checkbox::ChangeFunctionBinding(std::function<void(bool)> f) {
	toggleButton.ChangeFunctionBinding(f);
}

void Checkbox::ChangeKeybind(int keybind) {
	toggleButton.ChangeKeybind(keybind);
}

void Checkbox::ChangeText(std::string text) {
	label.ChangeText(text);
}

void Checkbox::ChangeTextSize(int size) {
	label.ChangeTextSize(size);
}

void Checkbox::ChangeValue(bool val) {
	toggleButton.ChangeValue(val);
}

bool Checkbox::GetValue() {
	return toggleButton.GetValue();
}

void Checkbox::Playsound() {
	toggleButton.Playsound();
}

void Checkbox::CallBoundFunction() {
	toggleButton.CallBoundFunction();
}

void Checkbox::SetActive(bool state) {
	InputDrawable::SetActive(state);
	toggleButton.SetActive(state);
}
