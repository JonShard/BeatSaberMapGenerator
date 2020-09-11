#pragma once

#include "Map.hpp"

namespace OK {

const int c_lanes = 4;
const int c_floors = 3;
const int c_types = 9; 
const int c_colors = 2;

template <class T>
class TransitionMatrix {
public:
    //                                     LaneFrom  LaneTo
    T m_matrix[c_colors][c_types][c_floors][c_lanes][c_lanes];
    TransitionMatrix();
    TransitionMatrix(std::string file);
    
    void loadFromFile(std::string file);
    void saveToFile(std::string file);
    void print();
};

} // namespace OK