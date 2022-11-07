#include "Color.h"

std::string Color::toString() {
	char buffer[12];
	sprintf_s(buffer, "%3u-%3u-%3u", r, g, b);

	return buffer;
}