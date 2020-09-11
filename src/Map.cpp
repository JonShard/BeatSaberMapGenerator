#include "../include/OK/Map.hpp"

namespace OK {
// for convenience
using json = nlohmann::json;

Map::Map() {}

Map::Map(const std::string fileName) {
    m_fileName = fileName;
    m_notes = std::vector<Note>();
    load(fileName);
}

void Map::load(const std::string fileName) {
   // read a JSON file
    std::ifstream file(fileName);
    json jsonMap;
    file >> jsonMap;
    
    jsonMap.at("_version").get_to(m_version);
    
    json jsonNotes = jsonMap["_notes"];

    for (json jn : jsonNotes) {
        Note n {
            jn.at("_time").get_to(n.time),
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

    json jsNotes;
    json jsonMap;
    
    for (Note n : m_notes) {
        printf("Adding note at time: %f\n", n.time);
        json jn;
        jn["_time"] = n.time;
        jn["_lineIndex"] = n.lineIndex;
        jn["_lineLayer"] = n.lineLayer;
        jn["_type"] = n.type;
        jn["_cutDirection"] = n.cutDirection;
        jsNotes.push_back(jn);
    }

    jsonMap["_version"] = "2.0.0";
    jsonMap["_notes"] = jsNotes;
    std::ofstream out(m_fileName);
    out << jsonMap;
    out.close();
}

void Map::print() {
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
        printf("Note: %f\t%s\t%s\t\n", n.time, (n.type == Type::BLUE)?"BLUE":"RED", dir.data());
    }
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