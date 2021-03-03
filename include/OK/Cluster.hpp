#pragma once

#include <vector>
#include "Note.hpp"


namespace OK {

const int c_maxNotesInCluster = 12; // Maximum number of notes a cluster can contain. 4 lanes x 3 layers = 12 

struct Cluster {
    std::vector<Note> m_notes;

    Cluster();

    Cluster getNotesOfType(Type type);
    
    bool isMultiColor();    // Returns true if the cluster contains both colors.

    void print();
    Cluster operator+=(Note n);
    Cluster operator+=(std::vector<Note> notes);
};

} // Namespace OK
