#include "../include/OK/Song.hpp"

namespace OK {

Song::Song() {
    m_path = "";
    // TODO Info
    m_maps = std::vector<Map>();
    m_notations = std::vector<Notation>();
}

Song::Song(std::string path) {
    m_path = path;
    // TODO Info
    m_maps = std::vector<Map>();
    m_notations = std::vector<Notation>();
}


void Song::loadInfo(std::string fileName) {

}

void Song::saveInfo() {

}


Map Song::getMap(const std::string name) {
    auto it = std::find_if(m_maps.begin(), m_maps.end(), [name](const Map & map){ return map.m_fileName == name; });

    if (it == m_maps.end()) {
        return Map();
    }
    return *it.base();
}

void Song::addMap(Map map) {

}

bool Song::loadMap(const std::string fileName) {
    return false;
}

bool Song::saveMap(const std::string name) {
    Map map = getMap(name);
    if (map.m_notes.size() <= 0) {
        printf("Warning: Can not save map without any notes\n");
        return false;
    }
    map.save();
    return true;
}

void Song::removeMap(const std::string name) {

}


Notation Song::getNotation(const std::string name) {
    auto it = std::find_if(m_notations.begin(), m_notations.end(), [name](const Notation & notation){ return notation.m_name == name; });

    if (it == m_notations.end()) {
        return Notation();
    }
    return *it.base();
}

Notation Song::CreateNotationFromMap(Map map) {
    Notation notation(map.m_fileName);
    
    for (int i = 0; i < map.m_notes.size(); i++) {
        std::vector<Note> cluster = map.getNotesInCluster(i);
        Keyframe k;
        k.id = notation.m_keyframes.size();
        k.time = cluster[0].m_time;
        k.concurrent = cluster.size();
        notation.m_keyframes.push_back(k);

        if (i + cluster.size() > map.m_notes.size() -1) {
            break;
        }
        i += cluster.size();
    }
    printf("Created notation from map\n");
    notation.print();
    return notation;
}

bool Song::loadNotation(const std::string fileName) {
    return false;
}

bool Song::saveNotation(const std::string name) {
    Notation notation = getNotation(name);
    if (notation.m_keyframes.size() <= 0) {
        printf("Warning: Can not save notation without any keyframes\n");
        return false;
    }
    notation.save();
    return true;
}


void Song::addNotation(Notation notation) {

}

void Song::removeNotation(const std::string name) {

}

    

} // namespace OK
