#pragma once
#include <stdlib.h>
#include <cstring> // strcmp
#include <string>

namespace OK::Util {

int rng(int min, int max);
bool isFileExtention(const std::string file, const std::string ext);


} // namespace OK::Util