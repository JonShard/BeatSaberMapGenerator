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

std::vector<Note> Map::GetNotesOfColorInCluster(std::vector<Note> cluster, Type type) {
    std::vector<Note> notes;
    for (Note n : cluster) {
        if (n.m_type == type)
            notes.push_back(n);
    }
    return notes;
}



Map::Map() {
    m_name = "";
    m_notes = std::vector<Note>();
}

Map::Map(const std::string name) {
    m_name = name;
    m_notes = std::vector<Note>();
}

bool Map::load(const std::string fileName, float bps) {
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
            jn.at("_time").get_to(n.m_time) / (bps / 60.0f),
            jn.at("_lineIndex").get_to(n.m_lineIndex),
            jn.at("_lineLayer").get_to(n.m_lineLayer),
            jn.at("_type").get_to(n.m_type),
            jn.at("_cutDirection").get_to(n.m_cutDirection)
        };
        m_notes.push_back(n);
    }
    return true;
    
}
void Map::save(const std::string fileName, float bps) {
    printf("Saving map with notes: %ld\n", m_notes.size());

    nlohmann::json jsNotes;
    nlohmann::json jsMap;
    
    for (Note n : m_notes) {
        //printf("Adding note at time (Adjusted for BPM): %f\n", n.m_time * (120.0f / 60.0f)); // TODO: dynamically get BPS (110)
        nlohmann::json jn;
        jn["_time"] = n.m_time * (bps / 60.0f);
        jn["_lineIndex"] = n.m_lineIndex;
        jn["_lineLayer"] = n.m_lineLayer;
        jn["_type"] = n.m_type;
        jn["_cutDirection"] = n.m_cutDirection;
        jn["_parentFactory"] = n.m_parentFactory.data();
        jsNotes.push_back(jn);
    }

    jsMap["_version"] = "2.0.0";
    jsMap["_notes"] = jsNotes;
    printf("Saving map file to: %s\n", fileName.data());
    std::ofstream out(fileName);
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

Note Map::getPreviousNoteOfColor(int noteNr, Type type) {
    if (m_notes.size() == 0 || noteNr <= 0)
        return Note();

    for (int i = noteNr - 1; i >= 0; i--) {
        if (m_notes[i].m_type == type) {
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