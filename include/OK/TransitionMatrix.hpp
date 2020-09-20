#pragma once

#include "Map.hpp"

namespace OK {

const int c_lanes = 4;
const int c_floors = 3;
const int c_types = 9; 
const int c_colors = 4; // RED, BLUE, RED_DUPLICATE, BOMB

template <class T>
struct TransitionMatrix {
    //         To       From      To       From     To        From      To       From
    T m_matrix[c_colors][c_colors][c_types][c_types][c_floors][c_floors][c_lanes][c_lanes];
    TransitionMatrix();
    TransitionMatrix(std::string file);
    
    void setNoteTransition(Note n, Note nn, T value);
    T getNoteTransition(Note n, Note nn);

    bool loadFromFile(std::string file);
    void saveToFile(std::string file);
    void print();
    TransitionMatrix<T> operator+= (TransitionMatrix m);
};

} // namespace OK