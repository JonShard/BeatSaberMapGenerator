
#include "../include/OK/Utilities.hpp"

namespace OK::Util {

int rng(int min, int max) {
    return (min + random() % max);
}

bool isFileExtention(const std::string file, const std::string ext) {
    int pos = file.find(".");
    if (pos < 0) 
        return false;
    std::string splitExt = file.substr(pos, file.length());
    return splitExt == ext;
}

} // namespace OK::Util