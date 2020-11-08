#include "../include/OK/Note.hpp"

namespace OK {

void Note::randomize() {
    m_type = (Type)(Util::rng(0, 100) < 60);   // Type 0 = red, 1 = blue
    m_cutDirection = (CutDirection)Util::rng(0, 8);    // Dot note is 9 none of those for now.
    
    // Notes are more likely to be towards the bottom.
    int layer = Util::rng(0, 100);
    if      (layer > 50) m_lineLayer = 0;
    else if (layer > 20) m_lineLayer = 1;
    else if (layer >= 0) m_lineLayer = 2;

    // Blue is more likely to be on the right.
    int number = Util::rng(0, 100);
    if      (number < 30) m_lineIndex = 3; 
    else if (number < 66) m_lineIndex = 2;
    else if (number < 85) m_lineIndex = 1;
    else m_lineIndex = 0;
    if (m_type == 0) { // If red, invert location.
        m_lineIndex = 3 - m_lineIndex;
    }
}

bool Note::isValid() {
    return (m_time > 0 && m_time < 100000 &&
        m_lineIndex >= 0 && m_lineIndex < 4 && 
        m_lineLayer >= 0 && m_lineLayer < 3 &&
        m_type >= 0 && m_type < 10 &&
        m_cutDirection >= 0 && m_cutDirection < 10);
}

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

int Note::getLongestLineLength() {
    int index = std::nearbyint(std::cos(Util::D2R * CutAngle[m_cutDirection]));
    int layer = std::nearbyint(std::sin(Util::D2R * CutAngle[m_cutDirection]));
    int length = 1;
    Note copy = *this;
    while (true) {
        copy.m_lineIndex += index;
        copy.m_lineLayer += layer;
        if (!copy.isValid()) 
            break;
        length++;
    }
    copy.m_lineIndex = m_lineIndex;
    copy.m_lineLayer = m_lineLayer;
    while (true) {
        copy.m_lineIndex -= index;
        copy.m_lineLayer -= layer;
        if (!copy.isValid()) 
            break;
        length++;
    }
    return length;
}

std::pair<int, int> Note::getPositionAbove() {
    int index = std::nearbyint(std::cos(Util::D2R * CutAngle[m_cutDirection]));
    int layer = std::nearbyint(std::sin(Util::D2R * CutAngle[m_cutDirection]));
    return std::pair<int, int>(m_lineIndex - index, m_lineLayer - layer);
}

std::pair<int, int> Note::getPositionBelow() {
    int index = std::nearbyint(std::cos(Util::D2R * CutAngle[m_cutDirection]));
    int layer = std::nearbyint(std::sin(Util::D2R * CutAngle[m_cutDirection]));
    return std::pair<int, int>(m_lineIndex + index, m_lineLayer + layer);
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

bool Note::isInLine(Note other) {
    return isOnSamePlane(other) && other.m_cutDirection == m_cutDirection;
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


void Note::print() {
    std::string dir;
    switch (m_cutDirection)
    {
        case CutDirection::UP: dir = "UP"; break;
        case CutDirection::DOWN: dir = "DOWN"; break;
        case CutDirection::LEFT: dir = "LEFT"; break;
        case CutDirection::RIGHT: dir = "RIGHT"; break;
        case CutDirection::UP_LEFT: dir = "UP LEFT"; break;
        case CutDirection::UP_RIGHT: dir = "UP RIGHT"; break;
        case CutDirection::DOWN_LEFT: dir = "DOWN LEFT"; break;
        case CutDirection::DOWN_RIGHT: dir = "DOWN RIGHT"; break;
        default: dir = "UNKNOWN"; break;
    }
    std::string type;
    switch (m_type)
    {
        case Type::RED: type = "RED"; break;
        case Type::BLUE: type = "BLUE"; break;
        case Type::RED_DUP: type = "RED_DUP"; break;
        case Type::BOMB: type = "BOMB"; break;
        default: type = "UNKNOWN"; break;
    }
    printf("Time: %f \tlineIndex: %d \tlineLayer: %d \ttype: %s \tcutDirection: %s, \tparentFactory: %s", 
            m_time, m_lineIndex, m_lineLayer, type, dir, m_parentFactory.c_str());
}

bool Note::operator==(Note other) {
    return m_lineIndex == other.m_lineIndex &&
        m_lineLayer == other.m_lineLayer &&
        m_type == other.m_type &&
        m_cutDirection == other.m_cutDirection;
}


} // Namespace OK