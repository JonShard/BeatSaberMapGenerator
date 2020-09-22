
#include "../include/OK/Utilities.hpp"

namespace OK::Util {

// Returns random numer in the range min()inclusive to max(exclusive). 
int rng(int min, int max) {
    return (min + random() % max);
}

bool isFileExtention(const std::string file, const std::string ext) {
    int pos = file.find_last_of(".");
    if (pos < 0) 
        return false;
    std::string splitExt = file.substr(pos, file.length());
    return splitExt == ext;
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


} // namespace OK::Util