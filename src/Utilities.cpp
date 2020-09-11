
#include "../include/OK/Utilities.hpp"

namespace OK::Util {

int rng(int min, int max) {
    return (min + random());
}

} // namespace OK::Util