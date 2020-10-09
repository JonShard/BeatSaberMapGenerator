// Notation is the representation of a user input on the keyboard. 
// Each of these will be generated into one or several notes, except if they are way too dense and some have to be removed.
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

struct Notation {
    std::string m_name;
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
    Notation operator=(Notation other);
};

} // Namespace OK
