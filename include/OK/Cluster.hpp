#pragma once

#include <vector>
#include "Note.hpp"


namespace OK {
    
struct Cluster {
    std::vector<Note> m_notes;

    Cluster();

    Cluster getNotesOfTypeInCluster(Type type);
    
    bool isMultiColor();    // Returns true if the cluster contains both colors.

    void print();
    Cluster operator+=(Note n);
    Cluster operator+=(std::vector<Note> notes);
};

} // Namespace OK
