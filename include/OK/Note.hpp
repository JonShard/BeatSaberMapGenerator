#pragma once

#include <cfenv> // round

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


} // Namespace OK
