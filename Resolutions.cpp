#include "Resolutions.h"

Resolutions::Resolution::Resolution(int width, int height) :
	w(width), h(height) {
}

std::string Resolutions::Resolution::toString() const {
	return std::to_string(w) + "x" + std::to_string(h);
}

int Resolutions::Resolution::GetWidth() const {
	return w;
}

int Resolutions::Resolution::GetHeight() const {
	return h;
}

bool Resolutions::Resolution::operator==(const Resolution& res) {
	return w == res.w && h == res.h;
}

int Resolutions::findResolutionIndex(Resolutions::Resolution& res) {
	for (int i = 0; i < Resolutions::SUPPORTED_RESOLUTIONS.size(); i++) {
		if (res == Resolutions::SUPPORTED_RESOLUTIONS[i]) {
			return i;
		}
	}
	return -1;
}
