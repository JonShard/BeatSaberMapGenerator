// Keyframe is a
#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../nlohmann/json.hpp"

namespace OK {

struct Keyframe {
    unsigned long id;
    float time;
    int concurrent;
};

class Notation {
private:
    std::string m_name;

public:
    std::vector<Keyframe> m_keyframes;

    Notation();
    Notation(std::string fileName);

    bool load(std::string fileName);
    void save();
    void print();

    std::string getName();
    Keyframe getNextKeyframe(float time);

    Notation operator+=(Keyframe k);
    Notation operator+=(std::vector<Keyframe> keyframes);
};

} // Namespace OK
