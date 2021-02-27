#include "../include/OK/Cluster.hpp"

namespace OK {

Cluster::Cluster() {
    m_notes = std::vector<Note>();
}

bool Cluster::isMultiColor() {
    bool red = false;
    bool blue = false;
    for (Note n : m_notes) {
        if (n.m_type = BLUE) blue = true;
        if (n.m_type = RED) red = true;
    }
    return red && blue;
}

Cluster Cluster::getNotesOfType(Type type) {
    Cluster notesOfType;
    for (Note n : m_notes) {
        if (n.m_type == type)
            notesOfType.m_notes.push_back(n);
    }
    return notesOfType;
}

void Cluster::print() {
    for (Note n : m_notes) {
        n.print();
    }
}

Cluster Cluster::operator+=(Note n) {
    m_notes.push_back(n);
    return *this;
}

Cluster Cluster::operator+=(std::vector<Note> notes) {
    for (Note n : notes) {
        m_notes.push_back(n);
    }
    return *this;
}

} // Namespace OK