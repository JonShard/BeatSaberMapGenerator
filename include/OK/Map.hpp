#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../nlohmann/json.hpp"

#include "Note.hpp"

namespace OK {
    
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
