// Map is a representaion of a single difficulty of a Beat Saber map.
#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../nlohmann/json.hpp"

namespace OK {

enum Type { BLUE, RED };
enum CutDirection { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };

struct Note {
    float time;
    int lineIndex;
    int lineLayer;
    int type;
    int cutDirection;
};


class Map {
private:
    std::string m_fileName;
    std::string m_version;

public:
    std::vector<Note> m_notes;
    
    Map();
    Map(std::string fileName);

    void load(std::string fileName);
    void save();
    void print();
    Map operator+=(Note n);
    Map operator+=(std::vector<Note> notes);

};

} // Namespace OK
