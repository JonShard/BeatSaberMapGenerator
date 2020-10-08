#include "../include/OK/Map.hpp"

namespace OK {

// Static funcitons:
bool Map::IsClusterMultiColor(std::vector<Note> cluster) {
    bool red = false;
    bool blue = false;
    for (Note n : cluster) {
        if (n.m_type = BLUE) blue = true;
        if (n.m_type = RED) red = true;
    }
    return red && blue;
}

std::vector<Note> Map::GetNotesOfColorInCluster(std::vector<Note> cluster, Type color) {
    std::vector<Note> notes;
    for (Note n : cluster) {
        if (n.m_type == color)
            notes.push_back(n);
    }
    return notes;
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


Map::Map() {
    m_name = "";
    m_notes = std::vector<Note>();
}

Map::Map(const std::string fileName) {
    m_name = fileName;
    m_notes = std::vector<Note>();
}

bool Map::load(const std::string fileName) {
    std::ifstream file(fileName);
    if (file.fail()) {
        return false;
    }
    nlohmann::json jsMap;
    file >> jsMap;
    
    jsMap.at("_version").get_to(m_version);
    
    nlohmann::json jsNotes = jsMap["_notes"];

    for (nlohmann::json jn : jsNotes) {
        Note n {
            jn.at("_time").get_to(n.m_time) / (120.0f / 60.0f),
            jn.at("_lineIndex").get_to(n.m_lineIndex),
            jn.at("_lineLayer").get_to(n.m_lineLayer),
            jn.at("_type").get_to(n.m_type),
            jn.at("_cutDirection").get_to(n.m_cutDirection)
        };
        m_notes.push_back(n);
    }
    return true;
}
void Map::save() {
    printf("Saving map with notes: %ld\n", m_notes.size());

    nlohmann::json jsNotes;
    nlohmann::json jsMap;
    
    for (Note n : m_notes) {
        //printf("Adding note at time (Adjusted for BPM): %f\n", n.m_time * (120.0f / 60.0f)); // TODO: dynamically get BPS (110)
        nlohmann::json jn;
        jn["_time"] = n.m_time * (120.0f / 60.0f);
        jn["_lineIndex"] = n.m_lineIndex;
        jn["_lineLayer"] = n.m_lineLayer;
        jn["_type"] = n.m_type;
        jn["_cutDirection"] = n.m_cutDirection;
        jsNotes.push_back(jn);
    }

    jsMap["_version"] = "2.0.0";
    jsMap["_notes"] = jsNotes;
    printf("Saving map file to: %s\n", m_name.data());
    std::ofstream out(m_name);
    out << jsMap;
    out.close();
}

void Map::print() {
    printf("Map: %s\n", m_name.data());
    std::string dir = "";
    for (Note n : m_notes) {
        switch (n.m_cutDirection)
        {
            case CutDirection::UP: dir = "UP"; break;
            case CutDirection::DOWN: dir = "DOWN"; break;
            case CutDirection::LEFT: dir = "LEFT"; break;
            case CutDirection::RIGHT: dir = "RIGHT"; break;
            case CutDirection::UP_LEFT: dir = "UP LEFT"; break;
            case CutDirection::UP_RIGHT: dir = "UP RIGHT"; break;
            case CutDirection::DOWN_LEFT: dir = "DOWN LEFT"; break;
            case CutDirection::DOWN_RIGHT: dir = "DOWN RIGHT"; break;
            default: dir = "UNKNOW"; break;
        }
        printf("Note: time: %f\ttype: %s\tcut direction: %s\t\n", n.m_time, (n.m_type == Type::BLUE)?"BLUE":"RED", dir.data());
    }
}

std::string Map::getName() { return m_name; }

float Map::getLatestTime() {
    if (m_notes.size() == 0)
        return 0;
    else 
        return m_notes.back().m_time;
}

std::vector<Note> Map::getNotesInCluster(int noteNr) {
    std::vector<Note> cluster;
    // Search forwards from note.
    for (int i = noteNr; i < m_notes.size(); i++) {
        if (std::abs(m_notes[i].m_time - m_notes[noteNr].m_time) < Config::generator.noteClusterTime) {
            cluster.push_back(m_notes[i]);
        }
        else {
            break;
        }
    }
    // Search bachwards from note.
    for (int i = noteNr -1; i > 0; i--) {
        if (std::abs(m_notes[noteNr].m_time - m_notes[i].m_time) < Config::generator.noteClusterTime) {
            cluster.push_back(m_notes[i]);
        }
        else {
            break;
        }
    }
    return cluster;
}

Note Map::getPreviousNoteOfColor(int noteNr, Type color) {
    if (m_notes.size() == 0 || noteNr <= 0)
        return Note();

    for (int i = noteNr - 1; i >= 0; i--) {
        if (m_notes[i].m_type == color) {
            return m_notes[i];
        }
    }
    return Note();
}


Map Map::operator+=(Note n) {
    m_notes.push_back(n);
    return *this;
}

Map Map::operator+=(std::vector<Note> notes) {
    for (Note n : notes) {
        m_notes.push_back(n);
    }
    return *this;
}


} // Namespace OK