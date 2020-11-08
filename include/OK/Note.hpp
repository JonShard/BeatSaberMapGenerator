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
    std::string m_parentFactory;

    void randomize();
    bool isValid();
    void invertNote(); // Invertes the color and cutdirection of the note. Blue down becomes red up. Position and time remain unchanged.
    void invertColor();
    void invertPosition(); // Invertes position of the note. Top left becomes bottom right. Everything else remains unchanged.
    void invertHorizontal();
    void invertVertical();
    int getLongestLineLength(); // Returns the amount of notes can be placed in a line depending on cut direction an position.
    std::pair<int, int> getPositionAbove(); // Returns the line index and layer of the above position relative to the notes cut direction.
    std::pair<int, int> getPositionBelow(); // Returns the line index and layer of the below position relative to the notes cut direction.
    bool isOnSamePlane(Note other);
    bool isInLine(Note other);
    bool isInCenter();
    bool isHorizontal();
    bool isVertical();
    bool isOppositeCutDirection(Note other);
    void print();
    bool operator==(Note other);
};


} // Namespace OK
