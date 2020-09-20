#pragma once
#include <stdlib.h>
#include <cstring> // strcmp
#include <string>

namespace OK::Util {

int rng(int min, int max);
bool isFileExtention(const std::string file, const std::string ext);
std::string removeFileExtention(const std::string file, const std::string ext);
unsigned int integerSumString(std::string str);

} // namespace OK::Util