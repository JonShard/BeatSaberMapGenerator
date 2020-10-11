#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cfenv> // round

#include "../nlohmann/json.hpp"

#include "Utilities.hpp"
#include "Config.hpp"

namespace OK {

enum Type { RED, BLUE, RED_DUP, BOMB}; // For some reason red appears twice in the types of note.
enum CutDirection       { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, DOT};
const int CutAngle[] =  { 90, 270,  180,  0,     135,     45,       225,       315,        -1 };

struct Note {
    float m_time;
    int m_lineIndex;
    int m_lineLayer;
    Type m_type;
    CutDirection m_cutDirection;

    void invertNote(); // Invertes the color and cutdirection of the note. Blue down becomes red up. Position and time remain unchanged.
    void invertColor();
    void invertPosition(); // Invertes position of the note. Top left becomes bottom right. Everything else remains unchanged.
    void invertHorizontal();
    void invertVertical();
    bool isInCenter();
    bool isHorizontal();
    bool isVertical();
    bool isOppositeCutDirection(Note other);
    bool isOnSamePlane(Note other);
    std::string toString();
};

enum DifficultyRank { EASY, NORMAL, HARD, EXPERT, EXPERTPLUS };

// Map is a representaion of a single difficulty of a Beat Saber map.
struct Map {
    std::string m_version;
    std::string m_name;
    std::string m_difficultyText;
    DifficultyRank m_difficultyRank;
    float m_noteJumpMovementSpeed;
    float m_noteJumpStartBeatOffset;
    std::vector<Note> m_notes;

    static bool IsClusterMultiColor(std::vector<Note> cluster);    // Returns true if the cluster contains both colors.
    static std::vector<Note> GetNotesOfColorInCluster(std::vector<Note> cluster, Type type);

    Map();
    Map(const std::string name);

    bool load(const std::string fileName, float bps);
    void save(const std::string fileName, float bps);
    void print();

    float getLatestTime();
    std::vector<Note> getNotesInCluster(int noteNr);        // Returns all notes in the same cluter as note in position noteNr (when there are several per "frame").
    Note getPreviousNoteOfColor(int noteNr, Type type);

    Map operator+=(Note n);
    Map operator+=(std::vector<Note> notes);
};

} // Namespace OK
