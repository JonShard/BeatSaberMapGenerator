#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../nlohmann/json.hpp"

namespace OK {

enum Type { BLUE, RED };
enum CutDirection {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};
enum Annotation { SINGLE, DOUBLE, SPECIAL };

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
    std::map<float, Annotation> m_annotation;

public:
    Map();
    Map(std::string fileName);

    std::vector<Note> Load(std::string fileName);
    void Save();
    void Print();
};

} // Namespace OK