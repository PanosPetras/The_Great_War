#include "Color.h"

#include <iomanip>
#include <sstream>

std::string Color::toString() {
        std::ostringstream os;
        os << std::setw(3) << r << '-' <<std::setw(3) << g << '-' << std::setw(3) << b;

	return os.str();
}
