#pragma once
#include <array>
#include <string>

namespace Resolutions {
	struct Resolution {
	public:
		Resolution(int width, int height);

		std::string toString() const;

		//Accessor functions
		int GetWidth() const;
		int GetHeight() const;

		bool operator == (const Resolution&);

	private:
		int w, h;
	};

	static const std::array<Resolution, 3> SUPPORTED_RESOLUTIONS = {Resolution(1280, 720), 
																	Resolution(1920, 1080), 
																	Resolution(2560, 1440)};

	int findResolutionIndex(Resolutions::Resolution&);
}