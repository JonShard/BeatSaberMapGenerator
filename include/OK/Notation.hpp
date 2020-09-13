// Keyframe is a
#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../nlohmann/json.hpp"

namespace OK {

enum KeyframeType { SINGLE, DOUBLE };

struct Keyframe {
    int id;
    float time;
    int concurrent;
    KeyframeType type;
};



} // Namespace OK
