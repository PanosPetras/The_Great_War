#pragma once
#include <array>
#include <string>

namespace Resolutions {

struct Resolution {
public:
    constexpr Resolution(int width, int height) : w(width), h(height) {}

    std::string toString() const;

    // Accessor functions
    int GetWidth() const;
    int GetHeight() const;

    constexpr auto operator<=>(const Resolution&) const noexcept = default;

private:
    int w, h;
};

constexpr std::array<Resolution, 3> SUPPORTED_RESOLUTIONS = {Resolution(1280, 720), Resolution(1920, 1080), Resolution(2560, 1440)};

constexpr std::array<int, 7> SUPPORTED_FRAMERATES = {30, 60, 75, 120, 144, 165, 240};

static inline constexpr unsigned not_found = static_cast<unsigned>(-1);
unsigned findResolutionIndex(Resolutions::Resolution&);
unsigned findFramerateIndex(int);
} // namespace Resolutions
