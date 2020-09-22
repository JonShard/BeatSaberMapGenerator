#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../nlohmann/json.hpp"

#include "Config.hpp"

namespace OK {

enum Type { RED, BLUE, RED_DUP, BOMB}; // For some reason red appears twice in the types of note.
enum CutDirection       { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, DOT};
const int CutAngle[] =  { 90, 270,  180,  0,     135,     45,       225,       315,        -1 };

struct Note {
    float time;
    int lineIndex;
    int lineLayer;
    int type;
    int cutDirection;

    std::string toString();
    void invertNote(); // Invertes the color and cutdirection of the note. Blue down becomes red up. Position and time remain unchanged.
    void invertPosition(); // Invertes position of the note. Top left becomes bottom right. Everything else remains unchanged.
};


// Map is a representaion of a single difficulty of a Beat Saber map.
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
    Note getPreviousNoteOfColor(int noteNr, Type color);

    Map operator+=(Note n);
    Map operator+=(std::vector<Note> notes);
};

} // Namespace OK
