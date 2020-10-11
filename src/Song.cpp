#include "../include/OK/Song.hpp"

namespace OK {

Song::Song() {
    m_path = "";
    // TODO Info
    m_maps = std::vector<Map>();
    m_notations = std::vector<Notation>();
}

Song::Song(std::string path) {
    m_path = Util::extractDirectoryFromPath(path);
    // TODO Info
    m_maps = std::vector<Map>();
    m_notations = std::vector<Notation>();
}

bool Song::loadSongFromDirectory(std::string path) {
    return false;
}

bool Song::saveSongToDirectory() {
    return false;
}




void Song::loadInfo(std::string fileName) {

}

void Song::saveInfo() {

}

Map Song::getMap(int index) {
    if (index < 0 || index > m_maps.size()) {
        return Map();
    }
    return m_maps[index];
}

Map Song::getMap(const std::string name) {
    auto it = std::find_if(m_maps.begin(), m_maps.end(), [name](const Map & map){ return map.m_name == name; });

    if (it == m_maps.end()) {
        return Map();
    }
    return *it.base();
}

int Song::addMap(Map map) {
    if (map.m_name.size() == 0) {
        printf("Warning: Can not add a map to song without a name\n");
        return -1;
    }
    if (map.m_notes.size() == 0) {
        printf("Warning: Can not add a map to song without any notes\n");
        return -1;
    }
    m_maps.push_back(map);
    return m_maps.size() -1;
}

int Song::loadMap(const std::string fileName) {
    if (!OK::Util::isFileExtention(fileName, c_mapFileExtention)) {
        return -1;
    }
    Map map(Util::extractFileNameFromPath(OK::Util::removeFileExtention(fileName, c_mapFileExtention)));
    if (!map.load(fileName, m_info.beatsPerMinute)) {
        printf("Warning: Failed to load file: %s\n", fileName.data());
        return -1;
    }
    if (map.m_notes.size() == 0) {
        printf("Warning: Can not load map file that doesn't contain any notes: %s\n", fileName.data());
        return -1;
    }
    m_maps.push_back(map);
    return m_maps.size()-1;
}

bool Song::saveMap(int index) {
    if (index < 0 || index > m_maps.size()) {
        printf("Warning: can not save map with out of bounds index %d, size: %ld", index, m_maps.size());
        return false;
    }
    return saveMap(m_maps[index].m_name);
}


bool Song::saveMap(const std::string name) {
    Map map = getMap(name);
    if (map.m_notes.size() <= 0) {
        printf("Warning: Can not save map without any notes\n");
        return false;
    }
    map.save(m_path + name + c_mapFileExtention,  m_info.beatsPerMinute);
    return true;
}

void Song::removeMap(const std::string name) {

}

Notation Song::getNotation(int index) {
    if (index < 0 || index > m_notations.size()) {
        return Notation();
    }
    return m_notations[index];
}

Notation Song::getNotation(const std::string name) {
    auto it = std::find_if(m_notations.begin(), m_notations.end(), [name](const Notation & notation){ return notation.m_name == name; });

    if (it == m_notations.end()) {
        return Notation();
    }
    return *it.base();
}

int Song::createNotationFromMap(Map map) {
    if (map.m_notes.size() == 0) {
        printf("Warning: Can not create notation from empty map\n");
        return -1;
    }
    
    Notation notation(map.m_name);

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
    m_notations.push_back(notation);
    return m_notations.size() -1;
}

int Song::loadNotation(const std::string fileName) {
    if (!OK::Util::isFileExtention(fileName, c_notationExtenton)) {
        return -1;
    }
    Notation notation(Util::extractFileNameFromPath(OK::Util::removeFileExtention(fileName, c_notationExtenton)));
    if (!notation.load(fileName)) {
        printf("Warning: Failed to load file: %s\n", fileName.data());
        return -1;
    }
    if (notation.m_keyframes.size() == 0) {
        printf("Warning: Can not load notaiton file that doesn't contain any keyframes: %s\n", fileName.data());
        return -1;
    }
    m_notations.push_back(notation);
    return m_notations.size() -1;
}

bool Song::saveNotation(int index) {
    if (index < 0 || index > m_notations.size()) {
        printf("Warning: can not save notation with out of bounds index %d, size: %ld", index, m_notations.size());
        return false;
    }
    return saveNotation(m_notations[index].m_name);
}

bool Song::saveNotation(const std::string name) {
    Notation notation = getNotation(name);
    if (notation.m_keyframes.size() <= 0) {
        printf("Warning: Can not save notation without any keyframes\n");
        return false;
    }
    notation.save(m_path + name + c_notationExtenton);
    return true;
}


int Song::addNotation(Notation notation) {
    if (notation.m_name.size() == 0) {
        printf("Warning: Can not add a notation to song without a name\n");
        return -1;
    }
    if (notation.m_keyframes.size() == 0) {
        printf("Warning: Can not add a notation to song without any keyframes\n");
        return -1;
    }
    m_notations.push_back(notation);
    return m_notations.size() -1;
}

void Song::removeNotation(const std::string name) {

}

    

} // namespace OK
