#pragma once
#include <stdlib.h>
#include <cstring> // strcmp
#include <string>

namespace OK::Util {

const float D2R = 0.01745329251;
const float R2D = 57.2958;

int rng(int min, int max);
float rng0To1();
bool isFileExtention(const std::string file, const std::string ext);
std::string extractDirectoryFromPath(std::string path);
std::string extractFileNameFromPath(std::string path);
std::string removeFileExtention(const std::string file, const std::string ext);
unsigned int integerSumString(std::string str);
int angleDelta(int a, int b);

} // namespace OK::Util