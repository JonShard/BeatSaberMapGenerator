#include "../include/OK/Map.hpp"

namespace OK {

// Static funcitons:
bool Map::IsClusterMultiColor(std::vector<Note> cluster) {
    bool red = false;
    bool blue = false;
    for (Note n : cluster) {
        if (n.type = BLUE) blue = true;
        if (n.type = RED) red = true;
    }
    return red && blue;
}

std::vector<Note> Map::GetNotesOfColorInCluster(std::vector<Note> cluster, Type color) {
    std::vector<Note> notes;
    for (Note n : cluster) {
        if (n.type == color)
            notes.push_back(n);
    }
    return notes;
}




void Note::print() {
    printf ("Time: %f \tlineIndex: %d \tlineLayer: %d \ttype: %d \tcutDirection: %d\n", 
            time, lineIndex, lineLayer, type, cutDirection);
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
            jn.at("_time").get_to(n.time) / (120.0f / 60.0f),
            jn.at("_lineIndex").get_to(n.lineIndex),
            jn.at("_lineLayer").get_to(n.lineLayer),
            jn.at("_type").get_to(n.type),
            jn.at("_cutDirection").get_to(n.cutDirection)
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
        printf("Adding note at time (Adjusted for BPM): %f\n", n.time * (120.0f / 60.0f)); // TODO: dynamically get BPS (110)
        nlohmann::json jn;
        jn["_time"] = n.time * (120.0f / 60.0f);
        jn["_lineIndex"] = n.lineIndex;
        jn["_lineLayer"] = n.lineLayer;
        jn["_type"] = n.type;
        jn["_cutDirection"] = n.cutDirection;
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
        switch (n.cutDirection)
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
        printf("Note: time: %f\ttype: %s\tcut direction: %s\t\n", n.time, (n.type == Type::BLUE)?"BLUE":"RED", dir.data());
    }
}

std::string Map::getName() { return m_name; }

float Map::getLatestTime() {
    if (m_notes.size() == 0)
        return 0;
    else 
        return m_notes.back().time;
}

std::vector<Note> Map::getNotesInCluster(int noteNr) {
    std::vector<Note> cluster;
    // Search forwards from note.
    for (int i = noteNr; i < m_notes.size(); i++) {
        if (m_notes[i].time - m_notes[noteNr].time < Config::generator.noteClusterTime) {
            cluster.push_back(m_notes[i]);
        }
        else {
            break;
        }
    }
    // Search bachwards from note.
    for (int i = noteNr -1; i > 0; i--) {
        if (m_notes[noteNr].time - m_notes[i].time < Config::generator.noteClusterTime) {
            cluster.push_back(m_notes[i]);
        }
        else {
            break;
        }
    }
    return cluster;
}

Note Map::getPreviousNoteOfColor(int noteNr, Type color) {
    if (m_notes.size() == 0 || noteNr < 0)
        return Note();

    for (int i = noteNr; i >= 0; i--) {
        if (m_notes[i].type = color) {
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