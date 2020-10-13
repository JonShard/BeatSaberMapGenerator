#include "../include/OK/Note.hpp"

namespace OK {


void Note::invertNote() {
    if      (m_type == BLUE) m_type = RED;
    else if (m_type == RED)  m_type = BLUE; // If bomb, keep as bomb.
    
    switch (m_cutDirection)
    {
    case UP: m_cutDirection = DOWN; break;
    case DOWN: m_cutDirection = UP; break;
    case LEFT: m_cutDirection = RIGHT; break;
    case RIGHT: m_cutDirection = LEFT; break;
    case UP_LEFT: m_cutDirection = DOWN_RIGHT; break;
    case UP_RIGHT: m_cutDirection = DOWN_LEFT; break;
    case DOWN_LEFT: m_cutDirection = UP_RIGHT; break;
    case DOWN_RIGHT: m_cutDirection = UP_LEFT; break;
    default: m_cutDirection = DOT; break;
    }
}

void Note::invertColor() {
    if      (m_type == BLUE) m_type = RED;
    else if (m_type == RED)  m_type = BLUE; // If bomb, keep as bomb.
}

void Note::invertPosition() {
    m_lineLayer = 2 - m_lineLayer;
    m_lineIndex = 3 - m_lineIndex;
}

void Note::invertHorizontal() {
    m_lineIndex = 3 - m_lineIndex;
}

void Note::invertVertical() {
    m_lineLayer = 2 - m_lineLayer;
}


bool Note::isInCenter() {
    return (m_lineLayer == 1 && (m_lineIndex == 1 || m_lineIndex == 2));
}

bool Note::isHorizontal() {
    return m_cutDirection == LEFT || m_cutDirection == RIGHT;
}

bool Note::isVertical() {
    return m_cutDirection == UP || m_cutDirection == DOWN;
}

bool Note::isOppositeCutDirection(Note other) {
    if (other.m_cutDirection == DOT) {
        if (m_cutDirection == DOT) {    // DOT is opposite of dot.
            return true;
        }
        return false;
    }
    return Util::angleDelta(CutAngle[other.m_cutDirection], CutAngle[m_cutDirection]) == 180;
}

bool Note::isOnSamePlane(Note other) {
    std::fesetround(FE_TONEAREST);
    int index = std::nearbyint(std::cos(Util::D2R * CutAngle[m_cutDirection]));
    int layer = std::nearbyint(std::sin(Util::D2R * CutAngle[m_cutDirection]));

    for (int i = - 4,l  = -4; i < 4; i++, l++)
    if (other.m_lineIndex == m_lineIndex + index * i && other.m_lineLayer == m_lineLayer + layer * l) {
        return true;
    }
    return false;
}

std::string Note::toString() {
    std::string s; 
    sprintf (&s[0], "Time: %f \tlineIndex: %d \tlineLayer: %d \ttype: %d \tcutDirection: %d", 
            m_time, m_lineIndex, m_lineLayer, m_type, m_cutDirection);
    return s;
}

} // Namespace OK