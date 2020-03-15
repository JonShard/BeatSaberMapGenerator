#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../nlohmann/json.hpp"

namespace OK {

enum Type { BLUE, RED };
enum CutDirection {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};
enum Notation { SINGLE, DOUBLE, SPECIAL };

struct Note {
    float time;
    int lineIndex;
    int lineLayer;
    int type;
    int cutDirection;
};

class Map {
    std::string m_fileName;
    std::string m_version;
    std::vector<Note> m_map;
    std::map<float, Notation> m_annotation;

public:
    Map();
    Map(std::string fileName);

    std::vector<Note> load(std::string fileName);
    void save();
    void print();
};

} // Namespace OK