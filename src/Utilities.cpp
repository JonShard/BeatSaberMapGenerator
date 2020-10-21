
#include "../include/OK/Utilities.hpp"

namespace OK::Util {

// Returns random numer in the range min()inclusive to max(exclusive). 
int rng(int min, int max) {
    if (max == 0)
        return 0;
    return (min + random() % max);
}

// Returns a random float between 0 and 1.
float rng0To1() {
    return rng(0, INT16_MAX) / (float)INT16_MAX;
}


bool isFileExtention(const std::string file, const std::string ext) {
    int pos = file.find_last_of(".");
    if (pos < 0) 
        return false;
    std::string splitExt = file.substr(pos, file.length());
    return splitExt == ext;
}

std::string extractDirectoryFromPath(std::string path) {
    int pos = path.find_last_of("/");
    if (pos < 0)
        return "";
    return path.substr(0, pos + 1); // + 1 to unclude the leading /
}

std::string extractFileNameFromPath(std::string path) {
    int pos = path.find_last_of("/");
    if (pos < 0)
        return "";
    return path.substr(pos + 1, path.size());
}

std::string removeFileExtention(const std::string file, const std::string ext) {
    int pos = file.find_last_of(ext);
    if (pos < 0) 
        return "";
    return file.substr(0, pos - ext.size() + 1);
}

unsigned int integerSumString(std::string str) {
    unsigned int sum = 0;
    for (char c : str) {
        sum += int(c);
    }
    return sum;
}

int angleDelta(int a, int b) {
    int m = std::abs(a - b);
    int n = std::abs(a - b + 360);
    int o = std::abs(a - b + 360);
    return std::min(m, std::min(n, o));
}


} // namespace OK::Util