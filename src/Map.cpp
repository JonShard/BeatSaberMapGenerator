#include "../include/OK/Map.hpp"

namespace OK {

Map::Map() {
    m_name = "";
    m_clusters = std::vector<Cluster>();
}

Map::Map(const std::string name) {
    m_name = name;
    m_clusters = std::vector<Cluster>();
}

int Map::getNoteCount() {
    int count = 0;
    for (Cluster c : m_clusters) {
        count += c.m_notes.size();
    }
    return count;
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

    Cluster c;
    for (nlohmann::json jn : jsNotes) {
        Note n {
            jn.at("_time").get_to(n.m_time) / (bps / 60.0f),
            jn.at("_lineIndex").get_to(n.m_lineIndex),
            jn.at("_lineLayer").get_to(n.m_lineLayer),
            jn.at("_type").get_to(n.m_type),
            jn.at("_cutDirection").get_to(n.m_cutDirection)
        };

        if (c.m_notes.size() > 0 && n.m_time - c.m_notes[0].m_time > Config::generator.noteClusterTime) {
            m_clusters.push_back(c);
            c = Cluster();
        }

        c.m_notes.push_back(n);
    }
    return true;
    
}
void Map::save(const std::string fileName, float bps) {
    printf("Saving map with notes: %d\n", getNoteCount());

    nlohmann::json jsNotes;
    nlohmann::json jsMap;
    
    for (Cluster c : m_clusters) {
        for (Note n : c.m_notes) {
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
    for (Cluster c : m_clusters) {
        c.print();
    }
}


float Map::getLatestTime() {
    if (m_clusters.size() == 0) {
        return 0;
    }
    else {
        return m_clusters.back().m_notes.back().m_time;
    }
}

Note Map::getLatestNote() {
    if (m_clusters.size() > 0) {
        return m_clusters.back().m_notes.back();
    }
    return Note{};
}

Note Map::getNoteAt(int index) {
    Note note;
    int i = 0;
    for (Cluster c : m_clusters) {
        for (Note n : c.m_notes){
            if (i == index) {
                return n;
            }
            i++;
        }
    }
    return note;
}

std::vector<Note> Map::getNotes() {
    std::vector<Note> notes;
    for (Cluster c : m_clusters) {
        for (Note n : c.m_notes){
            notes.push_back(n);
        }
    }
    return notes;
}


Note Map::getPreviousNoteOfColor(int noteNr, Type type) {
    if (m_clusters.size() == 0 || noteNr <= 0)
        return Note();

    for (int i = m_clusters.size() - 1; i >= 0; i--) {
        Cluster c = m_clusters[i].getNotesOfType(type);
        if (c.m_notes.size() > 0) {
            return c.m_notes.back();
        }
    }
    return Note{};
}


Map Map::operator+=(Note n) {
    Cluster c;
    c.m_notes.push_back(n);
    m_clusters.push_back(c);
    return *this;
}

Map Map::operator+=(std::vector<Note> notes) {
    for (Note n : notes) {
        Cluster c;
        c.m_notes.push_back(n);
        m_clusters.push_back(c);    
    }
    return *this;
}

Map Map::operator+=(Cluster c) {
    m_clusters.push_back(c);
    return *this;
}

Map Map::operator+=(std::vector<Cluster> clusters) {
    for (Cluster c : clusters) {
        m_clusters.push_back(c);
    }
    return *this;
}


} // Namespace OK