// Map is a representaion of a single difficulty of a Beat Saber map.
#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../nlohmann/json.hpp"

#include "Config.hpp"

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
    std::string m_name;
    std::string m_version;

public:
    static bool IsClusterMultiColor(std::vector<Note> cluster);    // Returns true if the cluster contains both colors.
    static std::vector<Note> GetNotesOfColorInCluster(std::vector<Note> cluster, Type color);

    std::vector<Note> m_notes;
    
    Map();
    Map(const std::string fileName);

    bool load(const std::string fileName);
    void save();
    void print();

    std::string getName();
    float getLatestTime();
    std::vector<Note> getNotesInCluster(int noteNr);        // Returns all notes in the same cluter as note in position noteNr (when there are several per "frame").

    Map operator+=(Note n);
    Map operator+=(std::vector<Note> notes);
};

} // Namespace OK
