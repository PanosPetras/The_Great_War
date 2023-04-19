#include "Resolutions.h"

Resolutions::Resolution::Resolution(int width, int height) :
	w(width), h(height) {
}

std::string Resolutions::Resolution::toString() {
	return std::to_string(w) + "x" + std::to_string(h);
}

int Resolutions::Resolution::GetWidth() const {
	return w;
}

int Resolutions::Resolution::GetHeight() const {
	return h;
}
