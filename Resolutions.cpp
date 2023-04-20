#include "Resolutions.h"

namespace Resolutions {

std::string Resolution::toString() const {
    return std::to_string(w) + 'x' + std::to_string(h);
}

int Resolutions::Resolution::GetWidth() const {
    return w;
}

int Resolutions::Resolution::GetHeight() const {
    return h;
}

unsigned findResolutionIndex(Resolutions::Resolution& res) {
    for (unsigned i = 0; i < Resolutions::SUPPORTED_RESOLUTIONS.size(); i++) {
        if (res == Resolutions::SUPPORTED_RESOLUTIONS[i]) {
            return i;
        }
    }
    return not_found;
}

unsigned findFramerateIndex(int f) {
    for (unsigned i = 0; i < Resolutions::SUPPORTED_FRAMERATES.size(); i++) {
        if (f == Resolutions::SUPPORTED_FRAMERATES[i]) {
            return i;
        }
    }
    return not_found;
}

} // namespace Resolutions
