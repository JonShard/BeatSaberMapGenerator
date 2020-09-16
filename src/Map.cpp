#include "../include/OK/Map.hpp"

namespace OK {

Map::Map() {
    m_name = "";
    m_notes = std::vector<Note>();
}

Map::Map(const std::string fileName) {
    m_name = fileName;
    m_notes = std::vector<Note>();
}

void Map::load(const std::string fileName) {
    std::ifstream file(fileName);
    nlohmann::json jsMap;
    file >> jsMap;
    
    jsMap.at("_version").get_to(m_version);
    
    nlohmann::json jsNotes = jsMap["_notes"];

    for (nlohmann::json jn : jsNotes) {
        Note n {
            jn.at("_time").get_to(n.time) / (110.0f / 60.0f),
            jn.at("_lineIndex").get_to(n.lineIndex),
            jn.at("_lineLayer").get_to(n.lineLayer),
            jn.at("_type").get_to(n.type),
            jn.at("_cutDirection").get_to(n.cutDirection)
        };
        m_notes.push_back(n);
    }
}
void Map::save() {
    printf("Saving map with notes: %ld\n", m_notes.size());

    nlohmann::json jsNotes;
    nlohmann::json jsMap;
    
    for (Note n : m_notes) {
        printf("Adding note at time (Adjusted for BPM): %f\n", n.time * (110.0f / 60.0f)); // TODO: dynamically get BPS (110)
        nlohmann::json jn;
        jn["_time"] = n.time * (110.0f / 60.0f);
        jn["_lineIndex"] = n.lineIndex;
        jn["_lineLayer"] = n.lineLayer;
        jn["_type"] = n.type;
        jn["_cutDirection"] = n.cutDirection;
        jsNotes.push_back(jn);
    }

    jsMap["_version"] = "2.0.0";
    jsMap["_notes"] = jsNotes;
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