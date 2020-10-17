#pragma once

#include "Map.hpp"

namespace OK {

const int c_lanes = 4;
const int c_floors = 3;
const int c_cutDirections = 9; 
const int c_types = 4; // RED, BLUE, RED_DUPLICATE, BOMB

// TransitionMatrix describes all allowed trasitions from one note to the next. 
// It is parsed from existing maps and stored on a file. It is used to validate the map.
template <class T>
struct TransitionMatrix {
    //         To       From      To       From     To        From      To       From
    T m_matrix[c_types][c_types][c_cutDirections][c_cutDirections][c_floors][c_floors][c_lanes][c_lanes];
    TransitionMatrix();
    TransitionMatrix(std::string file);
    
    void setNoteTransition(Note n, Note nn, T value);
    T getNoteTransition(Note n, Note nn);
    std::vector<T>getTransitionsFromNote(Note n);
    int getTransitionCountFromNote(Note n);
    int getNonZeroCount();
    int getTotalCount();
    void normalize();

    bool loadFromFile(std::string file);
    void saveToFile(std::string file);
    void print();
    TransitionMatrix<T> operator+= (TransitionMatrix m);
};

} // namespace OK